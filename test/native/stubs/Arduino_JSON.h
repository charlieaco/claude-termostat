// test/native/stubs/Arduino_JSON.h
#pragma once
#include "Arduino.h"
#include <vector>
#include <map>

class JSONVar {
    String _value;
    std::map<String, JSONVar> _properties;
    std::vector<JSONVar> _elements;
    bool _isArray = false;
    bool _isObject = false;

public:
    JSONVar() = default;
    JSONVar(const String& s) : _value(s) {}

    bool isNull() const { return _value.isEmpty(); }
    bool isValid() const { return !_value.isEmpty(); }
    bool hasOwnProperty(const String& key) const { return _properties.count(key) > 0; }

    size_t length() const {
        if (_isArray) return _elements.size();
        if (_isObject) return _properties.size();
        return _value.length();
    }

    String stringify() const { return _value; }
    const char* c_str() const { return _value.c_str(); }

    JSONVar operator[](const String& key) const {
        auto it = _properties.find(key);
        if (it != _properties.end()) return it->second;
        return JSONVar();
    }

    JSONVar operator[](const char* key) const {
        return (*this)[String(key)];
    }

    JSONVar operator[](const JSONVar& keyOrIndex) const {
        // If it looks like a number, treat as index
        if (keyOrIndex._isArray || keyOrIndex._isObject) {
            return JSONVar(); // invalid usage
        }
        // Try as index first if _isArray
        if (_isArray) {
            const char* str = keyOrIndex._value.c_str();
            int idx = std::atoi(str);
            if (idx >= 0 && idx < (int)_elements.size()) {
                return _elements[idx];
            }
        }
        // Fall back to property lookup
        return (*this)[keyOrIndex._value];
    }

    JSONVar operator[](int index) const {
        if (index >= 0 && index < (int)_elements.size()) {
            return _elements[index];
        }
        return JSONVar();
    }

    JSONVar keys() const {
        JSONVar result;
        result._isArray = true;
        for (const auto& kv : _properties) {
            result._elements.push_back(JSONVar(kv.first));
        }
        return result;
    }

    operator String() const { return _value; }
    operator const char*() const { return _value.c_str(); }
    operator double() const {
        const char* str = _value.c_str();
        return str ? std::strtod(str, nullptr) : 0.0;
    }
    operator int() const { return (int)(double)*this; }
};

class JSONClass {
public:
    JSONVar parse(const char* json_string) {
        return JSONVar(json_string);
    }

    JSONVar parse(const String& json_string) {
        return JSONVar(json_string);
    }

    String stringify(const JSONVar& var) {
        return var.stringify();
    }

    String typeof(const JSONVar& var) {
        if (!var.isValid()) return String("undefined");
        return String("object");
    }
};

inline JSONClass JSON;
