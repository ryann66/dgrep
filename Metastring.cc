#include <stdexcept>
#include <cstdlib>
#include <cstring>

#include "Metastring.h"
#include "globals.h"
#include "errors.h"

namespace metastring {

using std::string;
using std::ostream;
using std::logic_error;
using std::range_error;
using std::pair;

size_t MetastringHash::operator()(const Metastring& str) const {
    size_t hash = 5381;
    const char* cstr = str.str;
    while (*cstr) {
        hash = ((hash << 5) + hash) + *cstr; /* hash * 33 + *str.str */
        cstr++;
    }
    return hash;
}

Metastring::Metastring(const string& str) {
    strlen = str.length();
    this->str = new char[str.length() + 1];
    strcpy(this->str, str.c_str());
}

Metastring::Metastring(const char* str) {
    strlen = std::strlen(str);
    this->str = new char[std::strlen(str) + 1];
    strcpy(this->str, str);
}

Metastring::Metastring(char c) {
    this->str = new char[2];
    this->str[0] = c;
    this->str[1] = c;
    strlen = 1;
}

Metastring::Metastring(const Metastring& other) {
    strlen = other.strlen;
    str = new char[other.strlen + 1];
    strcpy(str, other.str);
    backrefs = other.backrefs;
}

Metastring::Metastring() { }

Metastring& Metastring::operator=(const Metastring& other) {
    delete str;
    str = new char[other.strlen + 1];
    strcpy(str, other.str);
    strlen = other.strlen;
    backrefs = other.backrefs;
    return *this;
}

Metastring::~Metastring() {
    delete str;
}

bool operator==(const Metastring& lhs, const Metastring& rhs) {
    if (lhs.strlen != rhs.strlen || strcmp(lhs.str, rhs.str)) {
        return false;
    }
    return lhs.backrefs == rhs.backrefs;
}

bool operator<(const Metastring& lhs, const Metastring& rhs) {
    return strcmp(lhs.str, rhs.str) < 0;
}

Metastring operator+(const Metastring& lhs, char rhs) {
    if (lhs.strlen == maxLength) throw new truncation_error();
    Metastring m;
    m.str = new char[lhs.strlen + 2];
    strcpy(m.str, lhs.str);
    char* end = m.str + lhs.strlen;
    end[0] = rhs;
    end[1] = '\0';
    m.strlen = lhs.strlen + 1;
    m.backrefs = lhs.backrefs;
    return m;
}

Metastring operator+(const Metastring& lhs, const string& rhs) {
    size_t rlen = rhs.length();
    if (lhs.strlen + rlen > maxLength) throw new truncation_error();
    Metastring m;
    m.str = new char[lhs.strlen + rlen + 1];
    strcpy(m.str, lhs.str);
    char* end = m.str + lhs.strlen;
    strcpy(end, rhs.c_str());
    m.strlen = lhs.strlen + rlen;
    m.backrefs = lhs.backrefs;
    return m;
}

Metastring Metastring::appendBackref(unsigned char br) {
    // find backref
    auto iter = backrefs.find(br);
    if (iter == backrefs.end()) throw new nonmatching_error("Backref not defined");
    size_t start = (*iter).second.first, end = (*iter).second.second;
    if (end < start) throw new logic_error("Open backref");
    size_t len = end - start;
    if (len + strlen > maxLength) throw new truncation_error();
    
    // create new metastring and init
    Metastring m;
    m.backrefs = backrefs;
    m.strlen = strlen + len;
    m.str = new char[strlen + len + 1];
    strcpy(m.str, str);

    // append backref
    char* appstr = m.str + strlen;
    for (size_t i = start; i < end; i++) {
        *appstr++ = str[i];
    }
    *appstr = '\0';
    return m;
}

std::ostream& operator<<(std::ostream& os, const Metastring& ms) {
    os << ms.str;
    return os;
}

string Metastring::toString() const {
    return string(str);
}

Metastring Metastring::startBackrefLogging(unsigned char br) {
    Metastring m(*this);
    pair<size_t, size_t> p(strlen, 0);
    m.backrefs[br] = p;
    return m;
}

Metastring Metastring::endBackrefLogging(unsigned char br) {
    Metastring m(*this);
    auto iter = m.backrefs.find(br);
    if (iter == backrefs.end()) throw new logic_error("Undefined backref");
    iter->second.second = m.strlen;
    return m;
}

}  // namespace metastring