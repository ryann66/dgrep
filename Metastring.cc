#include <stdexcept>

#include "Metastring.h"
#include "LinkedStringPrivate.h"
#include "globals.h"
#include "truncation_error.h"

namespace metastring {

using std::string;
using std::ostream;
using std::logic_error;

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
    return lhs.toString() == rhs.toString();
}

bool operator<(const Metastring& lhs, const Metastring& rhs) {
    return lhs.toString() < rhs.toString();
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

Metastring::Metastring(LinkedStringNode* s) : str(s) {
    str->refCount++;
}

size_t MetastringHash::operator()(const Metastring& op) const {
    return op.str->hash;
}

}  // namespace metastring