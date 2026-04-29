// test/native/stubs/Arduino.h
#pragma once
#include <string>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <cstdarg>

#define HIGH 1
#define LOW  0
#define INPUT  0
#define OUTPUT 1
#define PROGMEM

class String {
    std::string _s;
public:
    String() = default;
    String(const char* s) : _s(s ? s : "") {}
    String(const std::string& s) : _s(s) {}
    String(int n) : _s(std::to_string(n)) {}
    String(double d) : _s(std::to_string(d)) {}

    const char* c_str() const { return _s.c_str(); }
    size_t length() const { return _s.length(); }
    bool isEmpty() const { return _s.empty(); }

    String substring(int from, int to = -1) const {
        if (to < 0) return String(_s.substr(from));
        return String(_s.substr(from, to - from));
    }

    String& operator+=(const String& o) { _s += o._s; return *this; }
    String& operator+=(const char* o)   { _s += o;    return *this; }
    String& operator+=(char c)          { _s += c;    return *this; }

    bool operator==(const String& o) const { return _s == o._s; }
    bool operator==(const char* o)   const { return _s == o; }
    bool operator!=(const String& o) const { return _s != o._s; }
    bool operator!=(const char* o)   const { return _s != o; }

    operator const char*() const { return _s.c_str(); }
    operator std::string() const { return _s; }
};

inline String operator+(String a, const String& b) { return a += b; }
inline String operator+(String a, const char* b)   { return a += b; }
inline String operator+(const char* a, String b)   { return String(a) += b; }

struct SerialClass {
    void begin(unsigned long) {}
    void print(const char*)   {}
    void print(const String&) {}
    void println(const char* s = "") { (void)s; }
    void println(const String&)      {}
    void printf(const char*, ...)    {}
};

inline SerialClass Serial;

inline void pinMode(int, int)         {}
inline void digitalWrite(int, int)    {}
inline int  digitalRead(int)          { return 0; }
inline void delay(unsigned long)      {}
inline unsigned long millis()         { return 0; }
inline void yield()                   {}
