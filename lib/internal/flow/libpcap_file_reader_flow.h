#ifndef LIBTS_SENDER_INTERNAL_FLOW_LIBPCAP_FILE_READER_FLOW_H_
#define LIBTS_SENDER_INTERNAL_FLOW_LIBPCAP_FILE_READER_FLOW_H_

#include <condition_variable>
#include <fstream>
#include <string>
#include <thread>
#include <vector>
#include "bit_manager_api.h"
#include "libflow.h"

namespace ltss
{
namespace flow
{
namespace libpcap_file_reader
{

enum class WorkThreadStates
{
    kReadNextFile = 0,
    kReading,
    kFail,
    kHangUp,
};

enum class FileTypes
{
    kPcap = 0,
    kPcapng,
};

struct FileContext
{
    std::ifstream fs;
    std::vector<uint8_t> read_buf;
    size_t file_idx = 0;
    size_t file_size = 0;

    FileTypes file_type = FileTypes::kPcap;
    bitmgr::ByteOrders byte_order = bitmgr::ByteOrders::kLe;
    int64_t timestamp_resolution = lflow::kMsInNs;
};

class LibpcapFileReaderFlow : public lflow::IFlow
{
public:
    explicit LibpcapFileReaderFlow(const std::string &flow_id);
    virtual ~LibpcapFileReaderFlow();

    virtual bool Init(const char *param_str, void *opaque);
    virtual void DeInit();
    virtual bool Modify(const char *param_str, void *opaque);

    virtual std::shared_ptr<lflow::IPacketList> Process(std::shared_ptr<lflow::IPacketList> in_packet_list);
    virtual bool Control(const char *type, const char *param_str, void *opaque);

private:
    void WorkThread();
    int64_t WorkThreadReadNextFileState();
    int64_t WorkThreadReadingState();
    int64_t WorkThreadFailState();
    int64_t WorkThreadHangUpState();

    bool ReadFile(size_t size);
    void CloseCurrFile();
    bool ParseHeader();

private:
    std::string flow_id_;

    std::vector<std::string> files_;
    bool real_time_;
    int64_t repeat_times_;

    std::thread work_thread_;
    bool work_thread_running_;
    std::mutex work_thread_wait_mutex_;
    std::condition_variable work_thread_wait_cond_;

    WorkThreadStates work_thread_state_;
    FileContext curr_file_;

    std::mutex out_packet_list_mutex_;
    std::shared_ptr<lflow::IPacketList> out_packet_list_;
    bool out_packet_list_empty_;
};

}
}
}

#endif // !LIBTS_SENDER_INTERNAL_FLOW_LIBPCAP_FILE_READER_FLOW_H_
