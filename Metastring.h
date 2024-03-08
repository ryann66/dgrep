#pragma once

#include <string>
#include <memory>
#include <iostream>

#include "LinkedString.h"

namespace metastring {

class Metastring;

struct MetastringHash {
   size_t operator()(const Metastring&) const;
};

/**
 * Simplified mutable string variant that is 
 * capable of storing backreference metadata and appending a reference to that metadata
*/
class Metastring {
 public:
    /**
     * Create a metastring with the given backing string
    */
    Metastring(const std::string& str);
    Metastring(char c);

    Metastring(const Metastring&);
    Metastring& operator=(const Metastring&);

    friend bool operator==(const Metastring&, const Metastring&);
    friend bool operator<(const Metastring&, const Metastring&);

    friend size_t MetastringHash::operator()(const Metastring&) const;

    ~Metastring();

    /**
     * Concats, if conflicts encountered in metadata, the latter's will be preserved
    */
    friend Metastring operator+(const Metastring& lhs, const Metastring rhs);
    Metastring& operator+=(const Metastring& rhs);

    /**
     * Prints the metastring to the ostream
    */
    friend std::ostream& operator<<(std::ostream&, const Metastring&);

    /**
     * Returns a string representation
    */
    std::string toString() const;

    /**
     * Flags the entire contents of this as a backreference
     * Does nothing if br == 0
    */
    Metastring& markBackref(unsigned char br);

    /**
     * Appends the value of the given backref onto this
     * Appends the most recent value of the backref, if multiple
     * Throws an error if there are no backrefs matching br
     * Does nothing if br == 0
    */
    Metastring& appendBackref(unsigned char br);
    
 private:
    Metastring(LinkedStringNode*);

    LinkedStringNode* str;
};

}  // namespace metastring