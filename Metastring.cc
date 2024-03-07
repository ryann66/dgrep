#include <stdexcept>

#include "Metastring.h"
#include "LinkedStringPrivate.h"
#include "globals.h"
#include "truncation_error.h"

namespace metastring {

using std::string;
using std::ostream;
using std::logic_error;

Metastring::Metastring(const Metastring& o) : str(o.str) {
    str->refCount++;
}

Metastring& Metastring::operator=(const Metastring& rhs) {
    if (--str->refCount == 0) delete str;
    str = rhs.str;
    str->refCount++;
}

bool operator<(const Metastring& lhs, const Metastring& rhs) {
    return false;
    // TODO
}

Metastring::~Metastring() {
    if (--str->refCount == 0) delete str;
}

Metastring operator+(const Metastring& lhs, const Metastring rhs) {
    Metastring r(new AppendingLinkedStringNode(lhs.str, rhs.str));
    r.str->refCount--;
}

Metastring& Metastring::operator+=(const Metastring& rhs) {
    LinkedStringNode* n = new AppendingLinkedStringNode(str, rhs.str);
    str->refCount--;
    str = n;
}

ostream& operator<<(ostream& os, Metastring ms) {
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
}

Metastring& Metastring::appendBackref(unsigned char br) {
    LinkedStringNode* b = str->find(br);
    if (b == nullptr) throw logic_error("Backref not found");
    LinkedStringNode* n = new BackrefLinkedStringNode(str, b, br);
    str->refCount--;
    str = n;
}

Metastring::Metastring(LinkedStringNode* s) : str(s) {
    str->refCount++;
}

}  // namespace metastring