#pragma once

#include <memory>
#include <cstddef>
#include <ostream>

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
    virtual size_t length() = 0;

    /**
     * Returns a pointer to the given backref, or a null pointer if the backref does not exist
    */
    virtual std::shared_ptr<LinkedStringNode> find(unsigned char br);

    /**
     * Prints this to the ostream
    */
    void print(std::ostream);

 protected:
    LinkedStringNode() = default;
    LinkedStringNode(LinkedStringNode&) = delete;
    LinkedStringNode& operator=(const LinkedStringNode&) = delete;
};

}  // namespace metastring