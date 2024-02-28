#include "Metastring.h"
#include "globals.h"

using std::string;

#define UNUSED_BACKREF ((size_t) -1)

Metastring::Metastring() : strlen(0) {
    backrefs = (new size_t[maxBackref * 2]) - 1;
    for (unsigned char i = 1; i <= maxBackref * 2; i++) {
        backrefs[i] = UNUSED_BACKREF;
    }
    str = new char[maxLength];
}

Metastring::Metastring(const std::string& str) : strlen(str.length()) {
    if (strlen >= maxLength) {
        this->str = nullptr;
        backrefs = ((size_t*) nullptr) - 1;
        return;
    }

    backrefs = (new size_t[maxBackref * 2]) - 1;
    for (unsigned char i = 1; i <= maxBackref * 2; i++) {
        backrefs[i] = UNUSED_BACKREF;
    }
    this->str = new char[maxLength];
    const char* s = str.c_str();
    for (size_t i = 0; i < strlen; i++) {
        this->str[i] = s[i];
    }
    this->str[strlen] = '\0';
}

Metastring::Metastring(const Metastring& o) : strlen(o.strlen) {
    if (o.invalid()) {
        str = nullptr;
        backrefs = ((size_t*) nullptr) - 1;
        return;
    }

    backrefs = (new size_t[maxBackref * 2]) - 1;
    for (unsigned char i = 1; i <= maxBackref * 2; i++) {
        backrefs[i] = o.backrefs[i];
    }
    str = new char[maxLength];
    const char* s = o.str;
    for (size_t i = 0; i < strlen; i++) {
        str[i] = s[i];
    }
    str[strlen] = '\0';
}

Metastring& Metastring::operator=(const Metastring& rhs) {
    if (rhs.invalid()) {
        delete str;
        str = nullptr;
        return;
    }

    for (unsigned char i = 1; i <= maxBackref * 2; i++) {
        backrefs[i] = rhs.backrefs[i];
    }
    strlen = rhs.strlen;
    for (size_t i = 0; i < strlen; i++) {
        str[i] = rhs.str[i];
    }
    str[strlen] = '\0';
}

Metastring::~Metastring() {
    delete (backrefs + 1);
    delete str;
}

Metastring operator+(const Metastring& lhs, const Metastring rhs) {
    if (lhs.strlen + rhs.strlen >= maxLength || lhs.invalid() || rhs.invalid()) {
        return Metastring(nullptr);
    }
    Metastring m;
    m.strlen = lhs.strlen + rhs.strlen;
    for (size_t i = 0; i < lhs.strlen; i++) {
        m.str[i] = lhs.str[i];
    }
    char* str = m.str + lhs.strlen;
    for (size_t i = 0; i < rhs.strlen; i++) {
        str[i] = rhs.str[i];
    }
    
    for (unsigned char i = 1; i <= maxBackref * 2; i++) {
        if (rhs.backrefs[i] != UNUSED_BACKREF) {
            m.backrefs[i] = rhs.backrefs[i];
        }
        else {
            m.backrefs[i] = lhs.backrefs[i];
        }
    }
}

Metastring& Metastring::operator+=(const Metastring& rhs) {
    if (invalid()) return *this;
    if (rhs.invalid() || strlen + rhs.strlen >= maxLength) {
        delete str;
        str = nullptr;
        return *this;
    }

    char* s = str + strlen;
    for (size_t i = 0; i < rhs.strlen; i++) {
        s[i] = rhs.str[i];
    }
    strlen += rhs.strlen;
    str[strlen] = '\0';

    for (unsigned char i = 1; i <= maxBackref * 2; i++) {
        if (rhs.backrefs[i] != UNUSED_BACKREF) {
            backrefs[i] = rhs.backrefs[i];
        }
    }    
}

string Metastring::toString() const {
    return string(str);
}

char Metastring::firstChar() const {
    return str[0];
}

char Metastring::lastChar() const {
    return str[strlen - 1];
}

void Metastring::enableBackref(unsigned char br) {
    backrefs[br] = strlen;
}

void Metastring::disableBackref(unsigned char br) {
    backrefs[maxBackref + br] = strlen;
}

Metastring& Metastring::appendBackref(unsigned char br) {
    if (strlen + backrefs[maxBackref + br] - backrefs[br] >= maxLength) {
        delete str;
        str = nullptr;
        return *this;
    }
    for (size_t i = backrefs[br]; i < backrefs[maxBackref + br]; i++) {
        str[strlen++] = str[i];
    }
    str[strlen] = '\0';
}