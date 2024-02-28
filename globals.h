#pragma once

#include <set>

// maximum number backref that will be encountered
extern unsigned char maxBackref = 0;

// maximum length of matched strings, including null character
extern size_t maxLength = 256;

extern std::set<char> alphabet;