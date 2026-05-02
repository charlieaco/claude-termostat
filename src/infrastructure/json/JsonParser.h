#ifndef JSON_PARSER_H
#define JSON_PARSER_H

#include <Arduino_JSON.h>
#include <Arduino.h>

class JsonParser {
public:
    static bool extractValue(const String& jsonString, const String& key, String& result);
    static bool extractTemperatureValue(const String& jsonData, double& temperature);
    static bool extractOpenMeteoTemperature(const String& jsonData, double& temperature);
    static bool extractOpenMeteoApparentTemperature(const String& jsonData, double& apparentTemp);
    static bool extractOpenMeteoTime(const String& jsonData, String& time);
    static void printJsonStructure(const String& jsonString);
    
private:
    static bool isValidJson(const JSONVar& jsonObject);
};

#endif // JSON_PARSER_H 