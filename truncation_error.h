#pragma once

#include <stdexcept>
#include <string>

#include "globals.h"

class truncation_error : public std::runtime_error {
 public:
    truncation_error(const char* what) : runtime_error(what) {
        truncationCount++;
    }

    truncation_error(const std::string& what) : runtime_error(what) {
        truncationCount++;
    }
};