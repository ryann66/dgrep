#pragma once

#include <set>

class Alphabet {
 public:
    Alphabet();
    Alphabet(bool isNeg, const std::set<char>& chars);

    const std::set<char>::iterator begin() const { return chars.begin(); }
    const std::set<char>::iterator end() const { return chars.end(); }

    size_t size() { return chars.size(); }
    bool empty() { return chars.empty(); }

 private:
    std::set<char> chars;
};