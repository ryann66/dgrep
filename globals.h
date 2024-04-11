#pragma once

#include "Alphabet.h"

#define INFINITE_REPEAT 0xFFFFFFFF

// maximum length of matched strings
extern size_t maxLength;

extern Alphabet alphabet;

// count of truncation_errors that have been thrown
extern size_t truncationCount;