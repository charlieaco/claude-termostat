#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <WiFiClientSecureBearSSL.h>
#include <Arduino.h>

class HttpClient {
public:
    static bool getCurrentTime(String& currentTime);
    static bool getAuthToken(String& token);
    static bool getTemperatureData(const String& timestamp, const String& token, String& response);
    
private:
    static bool makeSecureRequest(const String& host, int port, const String& url, 
                                 const String& authHeader, String& response);
    static bool makeHttpRequest(const String& url, String& response);
    static String formatTimestamp(const String& rawDateTime);
    static String buildTemperatureUrl(const String& timestamp, const String& token);
};

#endif // HTTP_CLIENT_H 