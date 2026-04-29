// test/native/stubs/Arduino_JSON.h
#pragma once
#include "Arduino.h"
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <cctype>
#include <cstdlib>

// Forward declarations
class JSONVar;
static JSONVar parseValue(const std::string& json, size_t& pos);

class JSONVar {
public:
    enum class Type { UNDEFINED, NULL_T, BOOL, NUMBER, STRING, ARRAY, OBJECT };

    Type type = Type::UNDEFINED;
    double number = 0.0;
    std::string str;
    bool boolean = false;
    std::vector<JSONVar> array;
    std::map<std::string, JSONVar> object;

    JSONVar() : type(Type::UNDEFINED) {}

    bool hasOwnProperty(const char* key) const {
        return type == Type::OBJECT && object.count(key) > 0;
    }
    bool hasOwnProperty(const String& key) const {
        return hasOwnProperty(key.c_str());
    }

    size_t length() const {
        if (type == Type::ARRAY) return array.size();
        if (type == Type::OBJECT) return object.size();
        return 0;
    }

    JSONVar operator[](int idx) const {
        if (type == Type::ARRAY && idx >= 0 && idx < (int)array.size())
            return array[idx];
        return JSONVar();
    }
    JSONVar operator[](const char* key) const {
        if (type == Type::OBJECT) {
            auto it = object.find(key);
            if (it != object.end()) return it->second;
        }
        return JSONVar();
    }
    JSONVar operator[](const String& key) const {
        return (*this)[key.c_str()];
    }
    JSONVar operator[](const JSONVar& keyOrIndex) const {
        if (keyOrIndex.type == Type::NUMBER) {
            return (*this)[(int)keyOrIndex.number];
        } else if (keyOrIndex.type == Type::STRING) {
            return (*this)[keyOrIndex.str.c_str()];
        }
        return JSONVar();
    }

    // Keys — returns JSONVar array of string keys
    JSONVar keys() const {
        JSONVar result;
        result.type = Type::ARRAY;
        for (const auto& kv : object) {
            JSONVar k;
            k.type = Type::STRING;
            k.str = kv.first;
            result.array.push_back(k);
        }
        return result;
    }

    operator double() const {
        if (type == Type::NUMBER) return number;
        if (type == Type::STRING) return std::strtod(str.c_str(), nullptr);
        return 0.0;
    }
    operator String() const {
        if (type == Type::STRING) return String(str.c_str());
        if (type == Type::NUMBER) {
            std::ostringstream ss;
            ss << number;
            return String(ss.str().c_str());
        }
        return String("");
    }
    operator const char*() const { return str.c_str(); }
};

// ---- Minimal JSON parser ----

static void skipWs(const std::string& s, size_t& i) {
    while (i < s.size() && std::isspace((unsigned char)s[i])) i++;
}

static std::string parseString(const std::string& s, size_t& i) {
    // i points to opening "
    i++; // skip "
    std::string result;
    while (i < s.size() && s[i] != '"') {
        if (s[i] == '\\' && i + 1 < s.size()) {
            i++;
            switch (s[i]) {
                case '"': result += '"'; break;
                case '\\': result += '\\'; break;
                case '/': result += '/'; break;
                case 'n': result += '\n'; break;
                case 't': result += '\t'; break;
                case 'r': result += '\r'; break;
                default: result += s[i]; break;
            }
        } else {
            result += s[i];
        }
        i++;
    }
    if (i < s.size()) i++; // skip closing "
    return result;
}

static JSONVar parseValue(const std::string& json, size_t& pos) {
    skipWs(json, pos);
    JSONVar result;

    if (pos >= json.size()) return result;

    if (json[pos] == '"') {
        result.type = JSONVar::Type::STRING;
        result.str = parseString(json, pos);
    } else if (json[pos] == '{') {
        result.type = JSONVar::Type::OBJECT;
        pos++; // skip {
        skipWs(json, pos);
        while (pos < json.size() && json[pos] != '}') {
            skipWs(json, pos);
            if (json[pos] != '"') break;
            std::string key = parseString(json, pos);
            skipWs(json, pos);
            if (pos < json.size() && json[pos] == ':') pos++;
            JSONVar val = parseValue(json, pos);
            result.object[key] = val;
            skipWs(json, pos);
            if (pos < json.size() && json[pos] == ',') pos++;
        }
        if (pos < json.size()) pos++; // skip }
    } else if (json[pos] == '[') {
        result.type = JSONVar::Type::ARRAY;
        pos++; // skip [
        skipWs(json, pos);
        while (pos < json.size() && json[pos] != ']') {
            result.array.push_back(parseValue(json, pos));
            skipWs(json, pos);
            if (pos < json.size() && json[pos] == ',') pos++;
        }
        if (pos < json.size()) pos++; // skip ]
    } else if (json[pos] == 't' || json[pos] == 'f') {
        result.type = JSONVar::Type::BOOL;
        result.boolean = (json[pos] == 't');
        while (pos < json.size() && std::isalpha((unsigned char)json[pos])) pos++;
    } else if (json[pos] == 'n') {
        result.type = JSONVar::Type::NULL_T;
        while (pos < json.size() && std::isalpha((unsigned char)json[pos])) pos++;
    } else {
        // number
        result.type = JSONVar::Type::NUMBER;
        size_t start = pos;
        if (json[pos] == '-') pos++;
        while (pos < json.size() && (std::isdigit((unsigned char)json[pos]) ||
               json[pos] == '.' || json[pos] == 'e' || json[pos] == 'E' ||
               json[pos] == '+' || json[pos] == '-')) pos++;
        result.number = std::strtod(json.substr(start, pos - start).c_str(), nullptr);
    }
    skipWs(json, pos);
    return result;
}

// ---- JSON class ----

class JSONClass {
public:
    JSONVar parse(const char* json_string) {
        if (!json_string) return JSONVar();
        std::string s(json_string);
        size_t pos = 0;
        return parseValue(s, pos);
    }

    JSONVar parse(const String& json_string) {
        return parse(json_string.c_str());
    }

    String stringify(const JSONVar& var) {
        return (String)var;
    }

    String typeof(const JSONVar& var) {
        switch (var.type) {
            case JSONVar::Type::UNDEFINED: return String("undefined");
            case JSONVar::Type::NULL_T:    return String("null");
            case JSONVar::Type::BOOL:      return String("boolean");
            case JSONVar::Type::NUMBER:    return String("number");
            case JSONVar::Type::STRING:    return String("string");
            case JSONVar::Type::ARRAY:     return String("array");
            case JSONVar::Type::OBJECT:    return String("object");
        }
        return String("undefined");
    }
};

inline JSONClass JSON;
