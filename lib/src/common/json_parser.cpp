#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "common/json_parser.h"

bool ParseJsonToString(const rapidjson::Document &json_doc, std::string &str)
{
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    json_doc.Accept(writer);
    str = buffer.GetString();
    return true;
}

bool ParseJsonToString(const rapidjson::Value &json_val, std::string &str)
{
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    json_val.Accept(writer);
    str = buffer.GetString();
    return true;
}

bool ParseStringToJson(rapidjson::Document &json_doc, const std::string &str)
{
    rapidjson::Document tmp_doc;
    if (tmp_doc.Parse(str.c_str()).HasParseError())
    {
        return false;
    }

    json_doc.Swap(tmp_doc);
    return true;
}

bool SetJsonChildInt(rapidjson::Document &json_doc, const std::string &key, int val)
{
    auto &&allocator = json_doc.GetAllocator();
    json_doc.AddMember(rapidjson::Value(key.c_str(), allocator).Move(),
        rapidjson::Value(val).Move(),
        allocator);
}

bool SetJsonChildInt(rapidjson::Value &json_val, rapidjson::Document::AllocatorType &allocator, const std::string &key, int val)
{
    json_val.AddMember(rapidjson::Value(key.c_str(), allocator).Move(),
        rapidjson::Value(val).Move(),
        allocator);
}
