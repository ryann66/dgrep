#pragma once

#include <stdexcept>
#include <string>

class nonmatching_error : public std::runtime_error {
 public:
    nonmatching_error(const char* s) : runtime_error(s) { }
    nonmatching_error(std::string s) : runtime_error(s) { }
};