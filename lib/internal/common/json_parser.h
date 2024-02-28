#ifndef LIBTS_SENDER_INTERNAL_COMMON_JSON_PARSER_H_
#define LIBTS_SENDER_INTERNAL_COMMON_JSON_PARSER_H_

#include <cstdint>
#include <string>
#include "rapidjson/document.h"

bool ParseJsonToString(const rapidjson::Document &json_doc, std::string &str);
bool ParseJsonToString(const rapidjson::Value &json_val, std::string &str);
bool ParseStringToJson(rapidjson::Document &json_doc, const std::string &str);

bool IsJsonChildValid(const rapidjson::Document &json_doc, const std::string &key, rapidjson::Type type);
bool IsJsonChildValid(const rapidjson::Value &json_val, const std::string &key, rapidjson::Type type);

bool GetJsonChildNumber(const rapidjson::Document &json_doc, const std::string &key, int64_t &val);
bool GetJsonChildNumber(const rapidjson::Value &json_val, const std::string &key, int64_t &val);
bool GetJsonChildBool(const rapidjson::Document &json_doc, const std::string &key, bool &val);
bool GetJsonChildBool(const rapidjson::Value &json_val, const std::string &key, bool &val);
bool GetJsonChildString(const rapidjson::Document &json_doc, const std::string &key, std::string &val);
bool GetJsonChildString(const rapidjson::Value &json_val, const std::string &key, std::string &val);

void SetJsonChildNumber(rapidjson::Document &json_doc, const std::string &key, int64_t val);
void SetJsonChildNumber(rapidjson::Value &json_val, rapidjson::Document::AllocatorType &allocator, const std::string &key, int64_t val);
void SetJsonChildBool(rapidjson::Document &json_doc, const std::string &key, bool val);
void SetJsonChildBool(rapidjson::Value &json_val, rapidjson::Document::AllocatorType &allocator, const std::string &key, bool val);
void SetJsonChildString(rapidjson::Document &json_doc, const std::string &key, const std::string &val);
void SetJsonChildString(rapidjson::Value &json_val, rapidjson::Document::AllocatorType &allocator, const std::string &key, const std::string &val);

#endif // !LIBTS_SENDER_INTERNAL_COMMON_JSON_PARSER_H_
