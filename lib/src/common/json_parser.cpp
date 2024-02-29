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

bool ParseStringToJson(rapidjson::Document &json_doc, const char *str)
{
    rapidjson::Document tmp_doc;
    if ((!str) || (tmp_doc.Parse(str).HasParseError()))
    {
        return false;
    }

    json_doc.Swap(tmp_doc);
    return true;
}

bool IsJsonChildValid(const rapidjson::Document &json_doc, const std::string &key, rapidjson::Type type)
{
    if ((!json_doc.IsObject()) || (!json_doc.HasMember(key.c_str())) || (json_doc[key.c_str()].IsNull()))
    {
        return false;
    }

    const rapidjson::Value &json_val = json_doc[key.c_str()];
    return (json_val.GetType() == type);
}

bool IsJsonChildValid(const rapidjson::Value &json_val, const std::string &key, rapidjson::Type type)
{
    if ((!json_val.IsObject()) || (!json_val.HasMember(key.c_str())) || (json_val[key.c_str()].IsNull()))
    {
        return false;
    }

    const rapidjson::Value &json_child_val = json_val[key.c_str()];
    return (json_val.GetType() == type);
}

bool GetJsonChildNumber(const rapidjson::Document &json_doc, const std::string &key, int64_t &val)
{
    if (!IsJsonChildValid(json_doc, key, rapidjson::kNumberType))
    {
        return false;
    }

    val = json_doc[key.c_str()].GetInt();
    return true;
}

bool GetJsonChildNumber(const rapidjson::Value &json_val, const std::string &key, int64_t &val)
{
    if (!IsJsonChildValid(json_val, key, rapidjson::kNumberType))
    {
        return false;
    }

    val = json_val[key.c_str()].GetInt();
    return true;
}

bool GetJsonChildBool(const rapidjson::Document &json_doc, const std::string &key, bool &val)
{
    if ((!IsJsonChildValid(json_doc, key, rapidjson::kTrueType)) &&
        (!IsJsonChildValid(json_doc, key, rapidjson::kFalseType)))
    {
        return false;
    }

    val = json_doc[key.c_str()].GetBool();
    return true;
}

bool GetJsonChildBool(const rapidjson::Value &json_val, const std::string &key, bool &val)
{
    if ((!IsJsonChildValid(json_val, key, rapidjson::kTrueType)) &&
        (!IsJsonChildValid(json_val, key, rapidjson::kFalseType)))
    {
        return false;
    }

    val = json_val[key.c_str()].GetBool();
    return true;
}

bool GetJsonChildString(const rapidjson::Document &json_doc, const std::string &key, std::string &val)
{
    if (!IsJsonChildValid(json_doc, key, rapidjson::kStringType))
    {
        return false;
    }

    val = json_doc[key.c_str()].GetString();
    return true;
}

bool GetJsonChildString(const rapidjson::Value &json_val, const std::string &key, std::string &val)
{
    if (!IsJsonChildValid(json_val, key, rapidjson::kStringType))
    {
        return false;
    }

    val = json_val[key.c_str()].GetString();
    return true;
}

void SetJsonChildNumber(rapidjson::Document &json_doc, const std::string &key, int64_t val)
{
    auto &&allocator = json_doc.GetAllocator();
    json_doc.AddMember(rapidjson::Value(key.c_str(), allocator).Move(),
        rapidjson::Value(val).Move(),
        allocator);
}

void SetJsonChildNumber(rapidjson::Value &json_val, rapidjson::Document::AllocatorType &allocator, const std::string &key, int64_t val)
{
    json_val.AddMember(rapidjson::Value(key.c_str(), allocator).Move(),
        rapidjson::Value(val).Move(),
        allocator);
}

void SetJsonChildBool(rapidjson::Document &json_doc, const std::string &key, bool val)
{
    auto &&allocator = json_doc.GetAllocator();
    json_doc.AddMember(rapidjson::Value(key.c_str(), allocator).Move(),
        rapidjson::Value(val).Move(),
        allocator);
}

void SetJsonChildBool(rapidjson::Value &json_val, rapidjson::Document::AllocatorType &allocator, const std::string &key, bool val)
{
    json_val.AddMember(rapidjson::Value(key.c_str(), allocator).Move(),
        rapidjson::Value(val).Move(),
        allocator);
}

void SetJsonChildString(rapidjson::Document &json_doc, const std::string &key, const std::string &val)
{
    auto &&allocator = json_doc.GetAllocator();
    json_doc.AddMember(rapidjson::Value(key.c_str(), allocator).Move(),
        rapidjson::Value(val.c_str(), allocator).Move(),
        allocator);
}

void SetJsonChildString(rapidjson::Value &json_val, rapidjson::Document::AllocatorType &allocator, const std::string &key, const std::string &val)
{
    json_val.AddMember(rapidjson::Value(key.c_str(), allocator).Move(),
        rapidjson::Value(val.c_str(), allocator).Move(),
        allocator);
}

void SetJsonChildObject(rapidjson::Document &json_doc, const std::string &key, rapidjson::Value &json_child_val)
{
    auto &&allocator = json_doc.GetAllocator();
    json_doc.AddMember(rapidjson::Value(key.c_str(), allocator).Move(),
        json_child_val,
        allocator);
}

void SetJsonChildObject(rapidjson::Value &json_val, rapidjson::Document::AllocatorType &allocator, const std::string &key, rapidjson::Value &json_child_val)
{
    json_val.AddMember(rapidjson::Value(key.c_str(), allocator).Move(),
        json_child_val,
        allocator);
}

void SetJsonChildNull(rapidjson::Document &json_doc, const std::string &key)
{
    auto &&allocator = json_doc.GetAllocator();
    json_doc.AddMember(rapidjson::Value(key.c_str(), allocator).Move(),
        rapidjson::Value().SetNull().Move(),
        allocator);
}

void SetJsonChildNull(rapidjson::Value &json_val, rapidjson::Document::AllocatorType &allocator, const std::string &key)
{
    json_val.AddMember(rapidjson::Value(key.c_str(), allocator).Move(),
        rapidjson::Value().SetNull().Move(),
        allocator);
}
