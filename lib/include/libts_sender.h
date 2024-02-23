#ifndef LIBTS_SENDER_INCLUDE_LIBTS_SENDER_H_
#define LIBTS_SENDER_INCLUDE_LIBTS_SENDER_H_

#include <cstdint>
#include <memory>

#if defined(_WIN32)
#if defined(LIBTS_SENDER_API_EXPORT)
#define LIBTS_SENDER_API __declspec(dllexport)
#else
#define LIBTS_SENDER_API __declspec(dllimport)
#endif
#else
#define LIBTS_SENDER_API
#endif

namespace ltss
{

using NodeId = int64_t;

class ITsSender
{
public:
    virtual ~ITsSender() = default;

    virtual NodeId CreateNode(const char *param_str) = 0;
    virtual void DestroyNode(NodeId node_id) = 0;

    virtual bool StartNode(NodeId node_id) = 0;
    virtual void StopNode(NodeId node_id) = 0;

};

LIBTS_SENDER_API std::shared_ptr<ITsSender> CreateTsSender();

}

#endif // !LIBTS_SENDER_INCLUDE_LIBTS_SENDER_H_
