#include "LinkedStringPrivate.h"

using std::ostream;

namespace metastring {
    
LinkedStringNode* TerminatingLinkedStringNode::find(unsigned char br) const
{
    return nullptr;
}

void TerminatingLinkedStringNode::print(ostream& os) const {
    os << bstr;
}

void TerminatingLinkedStringNode::toString(std::string& rstr) const {
    rstr.append(bstr);
}

AppendingLinkedStringNode::AppendingLinkedStringNode(LinkedStringNode* pre, LinkedStringNode* suf) : 
        prefix(pre), suffix(suf), len(pre->length() + suf->length()) {
    prefix->refCount++;
    suffix->refCount++;
}

AppendingLinkedStringNode::~AppendingLinkedStringNode() {
    if (--prefix->refCount == 0) delete prefix;
    if (--suffix->refCount == 0) delete suffix;
}

LinkedStringNode* AppendingLinkedStringNode::find(unsigned char br) const {
    LinkedStringNode* b = suffix->find(br);
    if (b != nullptr) return b;
    return prefix->find(br);
}

void AppendingLinkedStringNode::print(ostream& os) const {
    prefix->print(os);
    suffix->print(os);
}

void AppendingLinkedStringNode::toString(std::string& rstr) const {
    prefix->toString(rstr);
    suffix->toString(rstr);
}

BackrefLinkedStringNode::BackrefLinkedStringNode(LinkedStringNode* prefix, LinkedStringNode* backref, unsigned char br) :
        prefix(prefix), backref(backref), br(br) {
    prefix->refCount++;
    backref->refCount++;
}

BackrefLinkedStringNode::BackrefLinkedStringNode(LinkedStringNode* backref, unsigned char br) :
        prefix(nullptr), backref(backref), br(br) {
    backref->refCount++;
}

BackrefLinkedStringNode::~BackrefLinkedStringNode() {
    if (prefix != nullptr && --prefix->refCount == 0) delete prefix;
    if (--backref->refCount == 0) delete backref;
}

size_t BackrefLinkedStringNode::length() const {
    return prefix->length() + backref->length();
}

LinkedStringNode* BackrefLinkedStringNode::find(unsigned char br) const {
    if (br == this->br) return static_cast<LinkedStringNode*>(const_cast<BackrefLinkedStringNode*>(this));
    LinkedStringNode* b = backref->find(br);
    if (b != nullptr) return b;
    if (prefix == nullptr) return nullptr;
    return prefix->find(br);
}

void BackrefLinkedStringNode::print(ostream& os) const {
    backref->print(os);
    if (prefix != nullptr) prefix->print(os);
}

void BackrefLinkedStringNode::toString(std::string& rstr) const {
    backref->toString(rstr);
    if (prefix != nullptr) prefix->toString(rstr);
}

}  // namespace metastring