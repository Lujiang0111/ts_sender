#include <fstream>
#include "common/json_parser.h"
#include "flow/pcap_file_reader_flow.h"

namespace ltss
{
namespace flow
{
namespace pcap_file_reader
{

/* param_json
{
    "pcap_file_path" : "string",
    "real_time_speed" : "boolean, true"
}
*/

PcapFileReaderFlow::PcapFileReaderFlow(const std::string &flow_id) :
    flow_id_(flow_id),
    real_time_speed_(true),
    work_thread_running_(false),
    out_packet_list_empty_(true)
{

}

PcapFileReaderFlow::~PcapFileReaderFlow()
{
    DeInit();
}

bool PcapFileReaderFlow::Init(const char *param_str, void *opaque)
{
    rapidjson::Document json_doc;
    if (!ParseStringToJson(json_doc, param_str))
    {
        return false;
    }

    if (!GetJsonChildString(json_doc, "pcap_file_path", pcap_file_path_))
    {
        return false;
    }

    if (!GetJsonChildBool(json_doc, "real_time_speed", real_time_speed_))
    {
        real_time_speed_ = true;
    }

    work_thread_running_ = true;
    work_thread_ = std::thread(&PcapFileReaderFlow::WorkThread, this);
    return true;
}

void PcapFileReaderFlow::DeInit()
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

bool PcapFileReaderFlow::Modify(const char *param_str, void *opaque)
{
    DeInit();
    return Init(param_str, opaque);
}

std::shared_ptr<lflow::IPacketList> PcapFileReaderFlow::Process(std::shared_ptr<lflow::IPacketList> in_packet_list)
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

bool PcapFileReaderFlow::Control(const char *type, const char *param_str, void *opaque)
{
    return true;
}

}
}
}
