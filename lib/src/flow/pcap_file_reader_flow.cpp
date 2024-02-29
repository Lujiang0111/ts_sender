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
    real_time_speed_(true)
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


    return true;
}

virtual void DeInit();
virtual bool Modify(const char *param_str, void *opaque);

virtual std::shared_ptr<lflow::IPacketList> Process(std::shared_ptr<lflow::IPacketList> in_packet_list);
virtual bool Control(const char *type, const char *param_str, void *opaque);

}
}
}
