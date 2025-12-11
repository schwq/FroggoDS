#pragma once

#ifndef __cplusplus
#error "This is a c++ header! Something is wrong :("
#endif

#if __cplusplus >= 202002L
#pragma message "Generating c++20 code :)"
#else
#error "This code is meant to run on c++ 20!"
#endif

#include <cstddef>
#include <format>
#include <iostream>
#include <string>

#define KB(kb) kb * 1024
#define MB(mb) KB(mb * 1024)
#define GB(gb) MB(gb * 1024)

typedef u_int64_t u64;
typedef u_int32_t u32;
typedef u_int16_t u16;
typedef u_int8_t u8;
typedef int64_t i64;
typedef int32_t i32;
typedef int16_t i16;
typedef int8_t i8;

#ifdef DEBUG_DS
#define DEBUG_ENABLED
#endif

#define BIT(a, n) ((a & (1 << n)) ? 1 : 0)

#ifdef __cpp_inline_variables
#define INLINE inline
#else
#define INLINE
#endif

#define BIT_SET(a, n, on)                                                      \
  {                                                                            \
    if (on)                                                                    \
      a |= (1 << n);                                                           \
    else                                                                       \
      a &= ~(1 << n);                                                          \
  }

#define BETWEEN(a, b, c) ((a >= b) && (a <= c))

#define FREE_FUN(p, fun)                                                       \
  do {                                                                         \
    if (p) {                                                                   \
      fun(p);                                                                  \
      p = nullptr;                                                             \
    }                                                                          \
  } while (0);

#define FREE(p) FREE_FUN(p, free)

#define SWP(a, b, tmp)                                                         \
  tmp = a;                                                                     \
  a = b;                                                                       \
  b = tmp;

namespace Froggo {
static std::string VerbosityToStr(int verbosity) {
  switch (verbosity) {
  case 0:
    return "[INF]: ";
  case -1:
    return "[ERR]: ";
  default:
    return "[WRN]: ";
  }
}

template <typename... T>
inline void Log(int verbosity, std::string_view fmt, T &&...args) {
  std::string message(std::vformat(fmt, std::make_format_args(args)...));
  std::string out = VerbosityToStr(verbosity) + message;
  std::cout << out << std::endl;
}

template <typename... T>
inline void LogInfo(std::string_view fmt, T &&...args) {
  Log(0, fmt, std::forward<T>(args)...);
}
template <typename... T>
inline void LogWarn(std::string_view fmt, T &&...args) {
  Log(1, fmt, std::forward<T>(args)...);
}
template <typename... T> inline void LogErr(std::string_view fmt, T &&...args) {
  Log(-1, fmt, std::forward<T>(args)...);
}
template <typename... T>
inline void NotImplemented(std::string_view fmt, T &&...args) {
  LogWarn("[NOT IMPLEMENTED]: {}", fmt, std::forward<T>(args)...);
}

#ifdef DEBUG_ENABLED
namespace Debug {
template <typename... T>
inline void Log(int verbosity, std::string_view fmt, T &&...args) {
  std::string message(std::vformat(fmt, std::make_format_args(args)...));
  std::string out = VerbosityToStr(verbosity) + message;
  std::cout << out << std::endl;
}

template <typename... T>
inline void LogInfo(std::string_view fmt, T &&...args) {
  Log(0, fmt, std::forward<T>(args)...);
}
template <typename... T>
inline void LogWarn(std::string_view fmt, T &&...args) {
  Log(1, fmt, std::forward<T>(args)...);
}
template <typename... T> inline void LogErr(std::string_view fmt, T &&...args) {
  Log(-1, fmt, std::forward<T>(args)...);
}
template <typename... T>
inline void NotImplemented(std::string_view fmt, T &&...args) {
  LogWarn("[NOT IMPLEMENTED]: {}", fmt, std::forward<T>(args)...);
}
} // namespace Debug
#endif
} // namespace Froggo
