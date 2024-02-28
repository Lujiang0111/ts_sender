#ifndef LIBTS_SENDER_INTERNAL_COMMON_JSON_PARSER_H_
#define LIBTS_SENDER_INTERNAL_COMMON_JSON_PARSER_H_

#include <string>
#include "rapidjson/document.h"

bool ParseJsonToString(const rapidjson::Document &json_doc, std::string &str);
bool ParseJsonToString(const rapidjson::Value &json_val, std::string &str);
bool ParseStringToJson(rapidjson::Document &json_doc, const std::string &str);

bool SetJsonChildInt(rapidjson::Document &json_doc, const std::string &key, int val);
bool SetJsonChildInt(rapidjson::Value &json_val, rapidjson::Document::AllocatorType &allocator, const std::string &key, int val);

#endif // !LIBTS_SENDER_INTERNAL_COMMON_JSON_PARSER_H_
