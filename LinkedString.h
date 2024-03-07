#pragma once

#include <cstddef>

namespace metastring {

/**
 * Immutable backing class for Metastring
 * Do not use
*/
class LinkedStringNode {
 public:
    virtual size_t size() = 0;

 protected:
    LinkedStringNode() = default;
    LinkedStringNode(LinkedStringNode&) = delete;
    LinkedStringNode& operator=(const LinkedStringNode&) = delete;
};

}  // namespace metastring