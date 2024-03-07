#pragma once

#include <memory>

#include "LinkedString.h"

namespace metastring {

/**
 * Immutable part of a LinkedString containing a literal
 * Terminating
*/
class TerminatingLinkedStringNode : public LinkedStringNode {

};

/**
 * Immutable part of a LinkedString
 * Appends two strings together
*/
class AppendingLinkedStringNode : public LinkedStringNode {

};

/**
 * Immutable part of a LinkedString
 * Like an AppendingLinkedStringNode but it marks the first string as a backreference
*/
class BackrefLinkedStringNode : public LinkedStringNode {

};

}  // namespace metastring