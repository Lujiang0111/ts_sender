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
    "real_time" : "boolean, true"
}
*/

PcapFileReaderFlow::PcapFileReaderFlow(const std::string &flow_id) :
    flow_id_(flow_id)
{

}

PcapFileReaderFlow::~PcapFileReaderFlow()
{
    DeInit();
}

bool PcapFileReaderFlow::Init(const char *param_str, void *opaque)
{
    

    return true;
}

virtual void DeInit();
virtual bool Modify(const char *param_str, void *opaque);

virtual std::shared_ptr<lflow::IPacketList> Process(std::shared_ptr<lflow::IPacketList> in_packet_list);
virtual bool Control(const char *type, const char *param_str, void *opaque);

}
}
}
