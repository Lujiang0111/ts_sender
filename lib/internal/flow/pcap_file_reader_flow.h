#ifndef LIBTS_SENDER_INTERNAL_FLOW_PCAP_FILE_READER_H_
#define LIBTS_SENDER_INTERNAL_FLOW_PCAP_FILE_READER_H_

#include <mutex>
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
    std::string flow_id_;

    std::string pcap_file_path_;
    bool real_time_speed_;

};


}
}
}

#endif // !LIBTS_SENDER_INTERNAL_FLOW_PCAP_FILE_READER_H_
