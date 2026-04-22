#include "HttpClient.h"
#include "config.h"
#include "JsonParser.h"

bool HttpClient::getCurrentTime(String& currentTime) {
    String rawDateTime;
    
    if (!makeHttpRequest(TIME_API_URL, rawDateTime)) {
        Serial.println("Failed to get current time from API");
        return false;
    }
    
    String extractedDateTime;
    if (!JsonParser::extractValue(rawDateTime, "datetime", extractedDateTime)) {
        Serial.println("Failed to extract datetime from response");
        return false;
    }
    
    currentTime = formatTimestamp(extractedDateTime);
    Serial.printf("Current time: %s\n", currentTime.c_str());
    return true;
}

bool HttpClient::getAuthToken(String& token) {
    String authHeader = "Basic " + String(METEO_LOGIN_CREDENTIALS);
    String response;
    
    if (!makeSecureRequest(METEO_LOGIN_HOST, METEO_LOGIN_PORT, "/api/v1/token", 
                          authHeader, response)) {
        Serial.println("Failed to get authentication token");
        return false;
    }
    
    if (!JsonParser::extractValue(response, "access_token", token)) {
        Serial.println("Failed to extract access token from response");
        return false;
    }
    
    Serial.println("Authentication token obtained successfully");
    return true;
}

bool HttpClient::getTemperatureData(const String& timestamp, const String& token, String& response) {
    String url = buildTemperatureUrl(timestamp, token);
    
    if (!makeSecureRequest(METEO_API_HOST, METEO_API_PORT, url, "", response)) {
        Serial.println("Failed to get temperature data");
        return false;
    }
    
    Serial.println("Temperature data retrieved successfully");
    return true;
}

bool HttpClient::makeSecureRequest(const String& host, int port, const String& url, 
                                  const String& authHeader, String& response) {
    std::unique_ptr<BearSSL::WiFiClientSecure> client(new BearSSL::WiFiClientSecure);
    client->setFingerprint(FINGERPRINT_SNI_CLOUDFLARESSL_COM);
    
    HTTPClient https;
    
    if (!https.begin(*client, host, port, url, true)) {
        Serial.printf("HTTPS connection failed to %s\n", host.c_str());
        return false;
    }
    
    if (authHeader.length() > 0) {
        https.addHeader("Authorization", authHeader);
    }
    
    Serial.printf("Making HTTPS request to %s%s\n", host.c_str(), url.c_str());
    int httpCode = https.GET();
    
    if (httpCode > 0) {
        Serial.printf("HTTPS response code: %d\n", httpCode);
        
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
            response = https.getString();
            https.end();
            return true;
        } else {
            Serial.printf("HTTPS request failed with code: %d\n", httpCode);
        }
    } else {
        Serial.printf("HTTPS request failed: %s\n", https.errorToString(httpCode).c_str());
    }
    
    https.end();
    return false;
}

bool HttpClient::makeHttpRequest(const String& url, String& response) {
    WiFiClient client;
    HTTPClient http;
    
    if (!http.begin(client, url)) {
        Serial.printf("HTTP connection failed to %s\n", url.c_str());
        return false;
    }
    
    Serial.printf("Making HTTP request to %s\n", url.c_str());
    int httpCode = http.GET();
    
    if (httpCode > 0) {
        Serial.printf("HTTP response code: %d\n", httpCode);
        
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
            response = http.getString();
            http.end();
            return true;
        } else {
            Serial.printf("HTTP request failed with code: %d\n", httpCode);
        }
    } else {
        Serial.printf("HTTP request failed: %s\n", http.errorToString(httpCode).c_str());
    }
    
    http.end();
    return false;
}

String HttpClient::formatTimestamp(const String& rawDateTime) {
    // Convert "2024-08-03T01:02:59.316944-03:00" to "2024-08-02T05:03:15Z"
    String formattedTime = rawDateTime.substring(0, 19); // Take first 19 characters
    formattedTime += "Z"; // Add Z suffix
    return formattedTime;
}

String HttpClient::buildTemperatureUrl(const String& timestamp, const String& token) {
    String url = "/";
    url += timestamp;
    url += "/t_2m:C/";
    url += LOCATION_LATITUDE;
    url += ",";
    url += LOCATION_LONGITUDE;
    url += "/json?access_token=";
    url += token;
    return url;
} 