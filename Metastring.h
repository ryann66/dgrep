#pragma once

#include <iostream>
#include <map>

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
    Metastring(const char*);
    Metastring(char c);

    Metastring(const Metastring&);
    Metastring& operator=(const Metastring&);
    ~Metastring();

    friend bool operator==(const Metastring&, const Metastring&);
    friend bool operator<(const Metastring&, const Metastring&);
    friend size_t MetastringHash::operator()(const Metastring&) const;

    /**
     * Returns a new metastring with the given character or string appended
    */
    friend Metastring operator+(const Metastring&, char);
    friend Metastring operator+(const Metastring&, const std::string&);

    /**
     * Appends the given backreference onto a new metastring
    */
    Metastring appendBackref(unsigned char);

    /**
     * Prints the metastring to the ostream
    */
    friend std::ostream& operator<<(std::ostream&, const Metastring&);

    /**
     * Returns a string representation without metadata
    */
    std::string toString() const;

    /**
     * Enables or disables logging appended characters/strings as backrefs
    */
    Metastring startBackrefLogging(unsigned char br);
    Metastring endBackrefLogging(unsigned char br);
    
 private:
    // WARNING: does not initialize str
    // You are responsible for initializing fields
    Metastring();

    char* str;
    size_t strlen;
    std::map<unsigned char, std::pair<size_t, size_t>> backrefs;
};

}  // namespace metastring