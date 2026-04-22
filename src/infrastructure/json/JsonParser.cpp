#include "JsonParser.h"

bool JsonParser::extractValue(const String& jsonString, const String& key, String& result) {
    JSONVar jsonObject = JSON.parse(jsonString);
    
    if (!isValidJson(jsonObject)) {
        Serial.println("JSON parsing failed!");
        return false;
    }
    
    if (jsonObject.hasOwnProperty(key)) {
        result = (String)jsonObject[key];
        Serial.printf("Extracted %s: %s\n", key.c_str(), result.c_str());
        return true;
    }
    
    Serial.printf("Key '%s' not found in JSON\n", key.c_str());
    return false;
}

bool JsonParser::extractTemperatureValue(const String& jsonData, double& temperature) {
    JSONVar jsonObject = JSON.parse(jsonData);
    
    if (!isValidJson(jsonObject)) {
        Serial.println("Temperature data parsing failed!");
        return false;
    }
    
    // Navigate through the nested JSON structure
    JSONVar dataArray = jsonObject["data"];
    if (dataArray.length() == 0) {
        Serial.println("No data array found");
        return false;
    }

    JSONVar firstDataPoint = dataArray[0];
    JSONVar coordinates = firstDataPoint["coordinates"];
    if (coordinates.length() == 0) {
        Serial.println("No coordinates found");
        return false;
    }

    JSONVar firstCoordinate = coordinates[0];
    JSONVar dates = firstCoordinate["dates"];
    if (dates.length() == 0) {
        Serial.println("No dates found");
        return false;
    }

    JSONVar firstDate = dates[0];
    temperature = (double)firstDate["value"];

    Serial.printf("Extracted temperature: %.2f°C\n", temperature);
    return true;
}

void JsonParser::printJsonStructure(const String& jsonString) {
    JSONVar jsonObject = JSON.parse(jsonString);
    
    if (!isValidJson(jsonObject)) {
        Serial.println("JSON parsing failed for structure print!");
        return;
    }
    
    JSONVar keys = jsonObject.keys();
    Serial.println("JSON Structure:");
    
    for (int i = 0; i < keys.length(); i++) {
        JSONVar value = jsonObject[keys[i]];
        Serial.printf("  %s: %s\n", (const char*)keys[i], (const char*)value);
    }
}

bool JsonParser::isValidJson(const JSONVar& jsonObject) {
    return JSON.typeof(jsonObject) != "undefined";
} 