#include <stdexcept>

#include "Metastring.h"
#include "LinkedStringPrivate.h"
#include "globals.h"
#include "truncation_error.h"

namespace metastring {

using std::string;
using std::ostream;
using std::logic_error;
using std::range_error;

Metastring::Metastring(const std::string& str) : str(new TerminatingLinkedStringNode(str)) { }

Metastring::Metastring(char c) : str(new TerminatingLinkedStringNode(c)) { }

Metastring::Metastring(const Metastring& o) : str(o.str) {
    str->refCount++;
}

Metastring& Metastring::operator=(const Metastring& rhs) {
    if (--str->refCount == 0) delete str;
    str = rhs.str;
    str->refCount++;
    return *this;
}

bool operator==(const Metastring& lhs, const Metastring& rhs) {
    if (lhs.str == rhs.str) return true;
    if (lhs.str->hash != rhs.str->hash || lhs.str->length() != rhs.str->length()) return false;
    Metastring::iterator ri(rhs.begin());
    for (auto li(lhs.begin()); li != lhs.end(); li++) {
        if (*li != *ri) return false;
    }
    return true;
}

bool operator<(const Metastring& lhs, const Metastring& rhs) {
    Metastring::iterator li(lhs.begin()), ri(rhs.begin()),
        le(lhs.end()), re(rhs.end());
    while (li != le && ri != re && *li == *ri) {
        li++;
        ri++;
    }
    if (ri == re) {
        return false;
    }
    if (li == le) {
        return true;
    }
    return *li < *ri;
}

Metastring::~Metastring() {
    if (--str->refCount == 0) delete str;
}

Metastring operator+(const Metastring& lhs, const Metastring rhs) {
    return Metastring(new AppendingLinkedStringNode(lhs.str, rhs.str));
}

Metastring& Metastring::operator+=(const Metastring& rhs) {
    LinkedStringNode* n = new AppendingLinkedStringNode(str, rhs.str);
    str->refCount--;
    str = n;
    return *this;
}

ostream& operator<<(ostream& os, const Metastring& ms) {
    ms.str->print(os);
    return os;
}

string Metastring::toString() const {
    string s;
    s.reserve(str->length());
    str->toString(s);
    return s;
}

Metastring& Metastring::markBackref(unsigned char br) {
    LinkedStringNode* b = new BackrefLinkedStringNode(str, br);
    str->refCount--;
    str = b;
    return *this;
}

Metastring& Metastring::appendBackref(unsigned char br) {
    LinkedStringNode* b = str->find(br);
    if (b == nullptr) throw logic_error("Backref not found");
    LinkedStringNode* n = new BackrefLinkedStringNode(str, b, br);
    str->refCount--;
    str = n;
    return *this;
}

Metastring::Metastring(const LinkedStringNode* s) : str(s) {
    str->refCount++;
}

size_t MetastringHash::operator()(const Metastring& op) const {
    return op.str->hash;
}

Metastring::iterator::iterator(const Metastring* p, bool isEnd) : parent(p) {
    if (isEnd) {
        curStr = nullptr;
        return;
    }
    parent->str->addTerminating(strings);
    if (strings.empty()) {
        curStr = nullptr;
    } else {
        curStr = strings.top();
        strings.pop();
    }
}

Metastring::iterator& Metastring::iterator::operator=(const iterator& rhs) {
    parent = rhs.parent;
    strings = rhs.strings;
    curStr = rhs.curStr;
    return *this;
}

Metastring::iterator& Metastring::iterator::operator++() {
    if (curStr == nullptr) throw range_error("Out of bounds");
    curStr++;
    if (curStr == nullptr && !strings.empty()) {
        curStr = strings.top();
        strings.pop();
    }
    return *this;
}

Metastring::iterator& Metastring::iterator::operator++(int) {
    return (*this).operator++();
}

bool Metastring::iterator::operator==(const iterator& other) const {
    if (this->parent != other.parent) return false;
    if (this->strings.size() != other.strings.size()) return false;
    return this->curStr == other.curStr;
}

Metastring::iterator::reference Metastring::iterator::operator*() const {
    if (curStr == nullptr) throw range_error("Out of bounds");
    return *curStr;
}

Metastring::iterator Metastring::begin() const {
    return iterator(this, false);
}

Metastring::iterator Metastring::end() const {
    return iterator(this, true);
}

}  // namespace metastring