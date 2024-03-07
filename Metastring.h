#pragma once

#include <string>
#include <memory>
#include <iostream>

#include "LinkedString.h"

namespace metastring {

/**
 * Simplified mutable string variant that is 
 * capable of storing backreference metadata and appending a reference to that metadata
*/
class Metastring {
 public:
    /**
     * Create a metastring with the given backing string
    */
    Metastring::Metastring(const std::string& str) : str(new TerminatingLinkedStringNode(str)) { }
    Metastring::Metastring(char c) : str(new TerminatingLinkedStringNode(c)) { }

    Metastring(const Metastring&);
    Metastring& operator=(const Metastring&);

    // deprecated (actually never implemented)
    friend bool operator<(const Metastring&, const Metastring&);

    ~Metastring();

    /**
     * Concats, if conflicts encountered in metadata, the latter's will be preserved
    */
    friend Metastring operator+(const Metastring& lhs, const Metastring rhs);
    Metastring& operator+=(const Metastring& rhs);

    /**
     * Prints the metastring to the ostream
    */
    friend std::ostream& operator<<(std::ostream&, Metastring);

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