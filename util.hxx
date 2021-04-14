#pragma once
#include <cstdarg>
#include <cstring>
#include <type_traits>
#include <vector>
#include <memory>
#include <map>
#include <set>
#include <string>
#include <sstream>

template<typename type_t>
const char* enum_to_string(type_t e) {
  return @enum_values(type_t) == e ...? @enum_names(type_t) : nullptr;
}

template<typename type_t>
std::optional<type_t> string_to_enum(const char* name) {
  return !strcmp(@enum_names(type_t), name) ...? @enum_values(type_t) : 
    std::optional<type_t> { };
}

inline std::string vformat(const char* pattern, va_list args) {
  va_list args_copy;
  va_copy(args_copy, args);

  int len = std::vsnprintf(nullptr, 0, pattern, args);
  std::string result(len, ' ');
  std::vsnprintf(result.data(), len + 1, pattern, args_copy);
 
  va_end(args_copy);
  return result;
}

inline std::string format(const char* pattern, ...) {
  va_list args;
  va_start(args, pattern);
  std::string s = vformat(pattern, args);
  va_end(args);
  return s;
}
