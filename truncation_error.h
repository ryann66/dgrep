#pragma once

#include <stdexcept>
#include <string>

#include "globals.h"

class truncation_error : public std::runtime_error {
 public:
    truncation_error() : runtime_error("truncated") {
        truncationCount++;
    }
};