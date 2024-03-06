#pragma once

#include <set>
#include <cstddef>

// maximum number backref that will be encountered
extern unsigned char maxBackref;

// maximum length of matched strings, including null character
extern size_t maxLength;

extern std::set<char> alphabet;

// count of truncation_errors that have been thrown
extern size_t count;