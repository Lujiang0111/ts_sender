#ifndef LIBTS_SENDER_INCLUDE_LIBTS_SENDER_H_
#define LIBTS_SENDER_INCLUDE_LIBTS_SENDER_H_

#if defined(_WIN32)
#if defined(LIBTS_SENDER_API_EXPORT)
#define LIBTS_SENDER_API __declspec(dllexport)
#else
#define LIBTS_SENDER_API __declspec(dllimport)
#endif
#else
#define LIBTS_SENDER_API
#endif

#endif // !LIBTS_SENDER_INCLUDE_LIBTS_SENDER_H_
