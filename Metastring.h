#pragma once

#include <string>

/**
 * Simplified mutable string variant that is 
 * capable of storing backreference metadata and appending a reference to that metadata
*/
class Metastring {
 public:
    /**
     * Create a metastring with the given backing string
    */
    Metastring();
    Metastring(const std::string& str);
    Metastring(char c);

    Metastring(const Metastring&);
    Metastring& operator=(const Metastring&);
    friend bool operator<(const Metastring&, const Metastring&);

    ~Metastring();

    /**
     * Concats, if conflicts encountered in metadata, the latter's will be preserved
    */
    friend Metastring operator+(const Metastring& lhs, const Metastring rhs);
    Metastring& operator+=(const Metastring& rhs);

    /**
     * Returns the backing string
    */
    std::string toString() const;

    /**
     * Return the first or last character, respectively
    */
    char firstChar() const;
    char lastChar() const;

    /**
     * Flags the entire contents of this as a backreference
     * Does nothing if br == 0
    */
    void markBackref(unsigned char br);

    /**
     * Appends the value of the given backref onto this
     * Appends the most recent value of the backref, if multiple
     * Throws an error if there are no backrefs matching br
     * Does nothing if br == 0
    */
    Metastring& appendBackref(unsigned char br);
    
 private:
	// invalid constructor
    Metastring(void*) : str(nullptr), backrefs(((size_t*) nullptr) - 1) { }

    // array of char locations for the start and end of backreferences
    // NOTE: start values can be accessed with the backref number
	// NOTE: ends of backrefs can be found by accessing backref number + maxBacckref
    size_t* backrefs;

    // backing string, maximum length is global.h::maxLength
    char* str;
    // length, excluding null char
    size_t strlen;
};