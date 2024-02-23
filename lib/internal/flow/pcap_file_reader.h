#ifndef LIBTS_SENDER_INTERNAL_FLOW_PCAP_FILE_READER_H_
#define LIBTS_SENDER_INTERNAL_FLOW_PCAP_FILE_READER_H_

#include <string>
#include "libflow.h"

namespace ltss
{
namespace flow
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

};

}
}

#endif // !LIBTS_SENDER_INTERNAL_FLOW_PCAP_FILE_READER_H_
