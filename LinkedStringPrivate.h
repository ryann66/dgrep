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
    TerminatingLinkedStringNode(const std::string& str);

    TerminatingLinkedStringNode(char c);

    virtual size_t length() const { return bstr.length(); }
    virtual const LinkedStringNode* find(unsigned char br) const;
    virtual void print(std::ostream&) const;
    virtual void toString(std::string&) const;
    virtual void addTerminating(std::stack<const char*>&) const;

 private:
    const std::string bstr;
};

/**
 * Immutable part of a LinkedString
 * Appends two strings together
*/
class AppendingLinkedStringNode : public LinkedStringNode {
 public:
    AppendingLinkedStringNode(const LinkedStringNode* pre, const LinkedStringNode* suf);
    virtual ~AppendingLinkedStringNode();

    virtual size_t length() const { return len; }
    virtual const LinkedStringNode* find(unsigned char br) const;
    virtual void print(std::ostream&) const;
    virtual void toString(std::string&) const;
    virtual void addTerminating(std::stack<const char*>&) const;

 private:
    const LinkedStringNode* prefix, *suffix;
    size_t len;
};

/**
 * Immutable part of a LinkedString
 * Like an AppendingLinkedStringNode but it marks the first string as a backreference
*/
class BackrefLinkedStringNode : public LinkedStringNode {
 public:
    // backref is suffix
    BackrefLinkedStringNode(const LinkedStringNode* prefix, const LinkedStringNode* backref, unsigned char br);
    BackrefLinkedStringNode(const LinkedStringNode* backref, unsigned char br);
    virtual ~BackrefLinkedStringNode();

    virtual size_t length() const;
    virtual const LinkedStringNode* find(unsigned char br) const;
    virtual void print(std::ostream&) const;
    virtual void toString(std::string&) const;
    virtual void addTerminating(std::stack<const char*>&) const;

 private:
    const LinkedStringNode* prefix, *backref;
    unsigned char br;
};

}  // namespace metastring