#ifndef LIBTS_SENDER_INTERNAL_FLOW_PCAP_FILE_READER_H_
#define LIBTS_SENDER_INTERNAL_FLOW_PCAP_FILE_READER_H_

#include <condition_variable>
#include <string>
#include <thread>
#include "libflow.h"

namespace ltss
{
namespace flow
{
namespace pcap_file_reader
{

class PcapFileReaderFlow : public lflow::IFlow
{
public:
    explicit PcapFileReaderFlow(const std::string &flow_id);
    virtual ~PcapFileReaderFlow();

    virtual bool Init(const char *param_str, void *opaque);
    virtual void DeInit();
    virtual bool Modify(const char *param_str, void *opaque);

    virtual std::shared_ptr<lflow::IPacketList> Process(std::shared_ptr<lflow::IPacketList> in_packet_list);
    virtual bool Control(const char *type, const char *param_str, void *opaque);

private:
    void WorkThread();

private:
    std::string flow_id_;

    std::string pcap_file_path_;
    bool real_time_speed_;

    std::thread work_thread_;
    bool work_thread_running_;
    std::mutex work_thread_wait_mutex_;
    std::condition_variable work_thread_wait_cond_;
    
    std::mutex out_packet_list_mutex_;
    std::shared_ptr<lflow::IPacketList> out_packet_list_;
    bool out_packet_list_empty_;
};

}
}
}

#endif // !LIBTS_SENDER_INTERNAL_FLOW_PCAP_FILE_READER_H_
