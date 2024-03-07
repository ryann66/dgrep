#pragma once

#include <string>

#include "LinkedString.h"

namespace metastring {

/**
 * Immutable part of a LinkedString containing a literal
 * Terminating
*/
class TerminatingLinkedStringNode : public LinkedStringNode {
 public:
    TerminatingLinkedStringNode(const std::string& str) : bstr(str) { }
    TerminatingLinkedStringNode(char c) : bstr(1, c) { }

    virtual size_t length() const { return bstr.length(); }
    virtual LinkedStringNode* find(unsigned char br) const;
    virtual void print(std::ostream&) const;
    virtual void toString(std::string&) const;

 private:
    std::string bstr;
};

/**
 * Immutable part of a LinkedString
 * Appends two strings together
*/
class AppendingLinkedStringNode : public LinkedStringNode {
 public:
    AppendingLinkedStringNode(LinkedStringNode* pre, LinkedStringNode* suf);
    virtual ~AppendingLinkedStringNode();

    virtual size_t length() const { return len; }
    virtual LinkedStringNode* find(unsigned char br) const;
    virtual void print(std::ostream&) const;
    virtual void toString(std::string&) const;

 private:
    LinkedStringNode* prefix, *suffix;
    size_t len;
};

/**
 * Immutable part of a LinkedString
 * Like an AppendingLinkedStringNode but it marks the first string as a backreference
*/
class BackrefLinkedStringNode : public LinkedStringNode {
 public:
    // backref is suffix
    BackrefLinkedStringNode(LinkedStringNode* prefix, LinkedStringNode* backref, unsigned char br);
    BackrefLinkedStringNode(LinkedStringNode* backref, unsigned char br);
    virtual ~BackrefLinkedStringNode();

    virtual size_t length() const;
    virtual LinkedStringNode* find(unsigned char br) const;
    virtual void print(std::ostream&) const;
    virtual void toString(std::string&) const;

 private:
    LinkedStringNode* prefix, *backref;
    unsigned char br;
};

}  // namespace metastring