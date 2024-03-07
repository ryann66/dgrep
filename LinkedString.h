#pragma once

#include <cstddef>
#include <ostream>
#include <string>
#include <stdexcept>

namespace metastring {

/**
 * Immutable backing class for Metastring
*/
class LinkedStringNode {
 public:
    virtual ~LinkedStringNode() { }

    /**
     * Length of the string, excluding null characters
    */
    virtual size_t length() const = 0;

    /**
     * Returns a pointer to the given backref, or a null pointer if the backref does not exist
    */
    virtual LinkedStringNode* find(unsigned char br) const = 0;

    /**
     * Prints this to the ostream
    */
    virtual void print(std::ostream&) const = 0;

    /**
     * appends this to the string
    */
    virtual void toString(std::string&) const = 0;

 protected:
    LinkedStringNode() : refCount(1) { }
    LinkedStringNode(LinkedStringNode&) = delete;
    LinkedStringNode& operator=(const LinkedStringNode&) = delete;

 public:
    size_t refCount;
};

}  // namespace metastring