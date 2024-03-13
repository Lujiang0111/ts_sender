#ifndef LIBTS_SENDER_INTERNAL_COMMON_LOGS_H_
#define LIBTS_SENDER_INTERNAL_COMMON_LOGS_H_

namespace logs
{

enum LogLevels
{
    kDebug = 0,
    kInfo,
    kWarning,
    kError
};

// LOGP uses an printf syntax
// Usage: LOGP(logs::kError, log << param1 << param2 << param3);
#define LOGP(level, fmt, ...)

// LOGS uses an iostream-like syntax, using the special 'log' symbol
// Usage: LOGS(logs::kError, log << param1 << param2 << param3);
#define LOGS(level, args)

}

#endif // !LIBTS_SENDER_INTERNAL_COMMON_LOGS_H_
