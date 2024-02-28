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
     * Toggles logging of backref characters
    */
    void toggleBackref(unsigned char br);

    /**
     * Appends the value of the given backref onto this
    */
    Metastring& appendBackref(unsigned char br);
    
 private:
    // array of char locations for the start and end of backreferences
    size_t* backrefStarts, *backrefEnds;

    // backing string, maximum length is global.h::maxLength
    char* str;
};