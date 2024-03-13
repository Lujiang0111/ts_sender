#include <fstream>
#include "common/json_parser.h"
#include "common/logs.h"
#include "flow/libpcap_file_reader_flow.h"

namespace ltss
{
namespace flow
{
namespace libpcap_file_reader
{

/* param_json
{
    "files": [
        "string"
    ],
    "real_time": "boolean, true",
    "repeat_times": "int, 0"
}
*/

LibpcapFileReaderFlow::LibpcapFileReaderFlow(const std::string &flow_id) :
    flow_id_(flow_id),
    real_time_(true),
    repeat_times_(0),
    work_thread_running_(false),
    work_thread_state_(WorkThreadStates::kReadNextFile),
    out_packet_list_empty_(true)
{

}

LibpcapFileReaderFlow::~LibpcapFileReaderFlow()
{
    DeInit();
}

bool LibpcapFileReaderFlow::Init(const char *param_str, void *opaque)
{
    rapidjson::Document json_doc;
    if (!ParseStringToJson(json_doc, param_str))
    {
        return false;
    }

    if (!IsJsonChildValid(json_doc, "pcap_files", rapidjson::kArrayType))
    {
        return false;
    }
    auto &&pcap_files_val = json_doc["pcap_files"];

    auto &&pcap_file_size = pcap_files_val.Size();
    if (0 == pcap_file_size)
    {
        return false;
    }

    files_.clear();
    for (rapidjson::SizeType idx = 0; idx < pcap_file_size; ++idx)
    {
        auto &&pcap_file_val = pcap_files_val[idx];
        std::string pcap_file = pcap_file_val.GetString();
        files_.push_back(pcap_file);
    }

    if (!GetJsonChildBool(json_doc, "real_time", real_time_))
    {
        real_time_ = true;
    }

    if (!GetJsonChildNumber(json_doc, "repeat_times", repeat_times_))
    {
        repeat_times_ = 0;
    }

    work_thread_running_ = true;
    work_thread_ = std::thread(&LibpcapFileReaderFlow::WorkThread, this);
    return true;
}

void LibpcapFileReaderFlow::DeInit()
{
    {
        std::lock_guard<std::mutex> lock(work_thread_wait_mutex_);
        work_thread_running_ = false;
    }
    work_thread_wait_cond_.notify_all();

    if (work_thread_.joinable())
    {
        work_thread_.join();
    }

    out_packet_list_ = nullptr;
    out_packet_list_empty_ = true;
}

bool LibpcapFileReaderFlow::Modify(const char *param_str, void *opaque)
{
    DeInit();
    return Init(param_str, opaque);
}

std::shared_ptr<lflow::IPacketList> LibpcapFileReaderFlow::Process(std::shared_ptr<lflow::IPacketList> in_packet_list)
{
    std::shared_ptr<lflow::IPacketList> out_packet_list;
    if (!out_packet_list_empty_)
    {
        std::lock_guard<std::mutex> lock(out_packet_list_mutex_);
        out_packet_list = out_packet_list_;
        out_packet_list_ = nullptr;
        out_packet_list_empty_ = true;
    }

    return out_packet_list;
}

bool LibpcapFileReaderFlow::Control(const char *type, const char *param_str, void *opaque)
{
    return true;
}

void LibpcapFileReaderFlow::WorkThread()
{
    int64_t sleep_ns = 0;
    curr_file_.file_idx = files_.size() - 1;
    work_thread_state_ = WorkThreadStates::kReadNextFile;
    while (work_thread_running_)
    {
        switch (work_thread_state_)
        {
        case WorkThreadStates::kReadNextFile:
            sleep_ns = WorkThreadReadNextFileState();
            break;
        case WorkThreadStates::kReading:
            sleep_ns = WorkThreadReadingState();
            break;
        case WorkThreadStates::kFail:
            sleep_ns = WorkThreadFailState();
            break;
        case WorkThreadStates::kHangUp:
            sleep_ns = WorkThreadHangUpState();
            break;
        default:
            work_thread_state_ = WorkThreadStates::kFail;
            break;
        }

        if (sleep_ns > 0)
        {
            std::unique_lock<std::mutex> lock(work_thread_wait_mutex_);
            work_thread_wait_cond_.wait_for(lock, std::chrono::nanoseconds(sleep_ns), [this] {return (!work_thread_running_); });
        }
    }

    // 清理
    CloseCurrFile();
}

int64_t LibpcapFileReaderFlow::WorkThreadReadNextFileState()
{
    CloseCurrFile();

    curr_file_.file_idx = (curr_file_.file_idx + 1) % files_.size();
    curr_file_.fs.open(files_[curr_file_.file_idx], std::ios::binary);
    if (!curr_file_.fs.is_open())
    {
        return lflow::kMsInNs;
    }

    curr_file_.fs.seekg(0, curr_file_.fs.end);
    int file_size = curr_file_.fs.tellg();
    if (file_size <= 0)
    {
        return lflow::kMsInNs;
    }
    curr_file_.file_size = static_cast<size_t>(file_size);

    if (!ParseHeader())
    {
        return lflow::kMsInNs;
    }

    work_thread_state_ = WorkThreadStates::kReading;
    return 0;
}

bool LibpcapFileReaderFlow::ReadFile(size_t size)
{
    if (!curr_file_.fs.is_open())
    {
        LOGS(logs::kError, log << "[" << files_[curr_file_] << "] " << "file stat error!");
        return false;
    }

    size_t curr_offset = static_cast<size_t>(curr_file_.fs.tellg());
    if (curr_file_.file_size - curr_offset < size)
    {
        LOGS(logs::kError, log << "[" << files_[curr_file_] << "] " << "file length not enough! require " << size <<
            "left " << curr_file_.file_size - curr_offset);
        return false;
    }

    if (curr_file_.read_buf.size() < size)
    {
        curr_file_.read_buf.resize(size + 1024);
    }

    curr_file_.fs.read(reinterpret_cast<char *>(&curr_file_.read_buf[0]), size);
    return true;
}

void LibpcapFileReaderFlow::CloseCurrFile()
{
    if (curr_file_.fs.is_open())
    {
        curr_file_.fs.close();
    }
}

bool LibpcapFileReaderFlow::ParseHeader()
{
    if (curr_file_.file_size < 4)
    {
        return false;
    }

    curr_file_.fs.seekg(0, curr_file_.fs.beg);
    uint32_t file_type_number = 0;
    curr_file_.fs.read(reinterpret_cast<char *>(&file_type_number), 4);

    switch (file_type_number)
    {
    case 0xA1B2C3D4:
    case 0xA1B23C4D:
    case 0xD4C3B2A1:
    case 0x4D3CB2A1:
        curr_file_.file_type = FileTypes::kPcap;
        break;
    default:
        LOGS(logs::kError, log << "[" << files_[curr_file_] << "] " << "error file type number : " << file_type_number);
        return false;
    }

    curr_file_.fs.seekg(0, curr_file_.fs.beg);
    switch (curr_file_.file_type)
    {
    case FileTypes::kPcap:
    {
        if (!ReadFile(24))
        {
            return false;
        }

        if (0xA1 == curr_file_.read_buf[0])
        {
            curr_file_.byte_order = bitmgr::ByteOrders::kBe;
        }
        else
        {
            curr_file_.byte_order = bitmgr::ByteOrders::kLe;
        }

        auto &&bm = bitmgr::CreateBitReader(&curr_file_.read_buf[0], 0, curr_file_.byte_order);
        int64_t magic_number = bm->Read4Bytes();
        if (0xA1B23C4D == magic_number)
        {
            curr_file_.timestamp_resolution = lflow::kSecInUs;
        }
        else
        {
            curr_file_.timestamp_resolution = lflow::kSecInNs;
        }

        break;
    }

    default:
        return false;
    }

    return true;
}

}
}
}
