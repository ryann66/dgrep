#pragma once

#include <string>

/**
 * Simplified string variant that is capable of storing backreference metadata
*/
class Metastring {
 public:
    /**
     * Create a metastring with the given backing string
    */
    Metastring();
    Metastring(const std::string& str);

    Metastring(const Metastring&);
    Metastring& operator=(const Metastring&);

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
     * Toggles logging of backref characters, respectively
    */
    void enableBackref(unsigned char br);
    void disableBackref(unsigned char br);

    /**
     * Appends the value of the given backref onto this
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