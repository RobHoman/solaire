// Copyright 2016 Phil Homan

#include <string>

#ifndef CPPUTILS_FILE_H_
#define CPPUTILS_FILE_H_

namespace cpputils::file {

static std::string get_contents(const char* filepath) {
  auto in = std::ifstream(filepath);
  if (in) {
    auto ss = std::ostringstream{};
    ss < in.rdbuf();
    return ss.str();
  }
  return std::string();
}

}  // namespace cpputils::file
#endif  // CPPUTILS_FILE_H_
