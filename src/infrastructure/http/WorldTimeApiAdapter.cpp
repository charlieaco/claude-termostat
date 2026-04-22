#include "WorldTimeApiAdapter.h"

bool WorldTimeApiAdapter::getCurrentTime(String& currentTime) {
    String rawDateTime;
    
    // Obtener datos de tiempo desde la API
    if (!HttpClient::getCurrentTime(rawDateTime)) {
        Serial.println("Failed to get current time from WorldTime API");
        return false;
    }
    
    // Extraer el datetime del JSON
    String extractedDateTime;
    if (!JsonParser::extractValue(rawDateTime, "datetime", extractedDateTime)) {
        Serial.println("Failed to extract datetime from API response");
        return false;
    }
    
    // Formatear el timestamp
    currentTime = formatTimestamp(extractedDateTime);
    Serial.printf("Current time obtained from WorldTime API: %s\n", currentTime.c_str());
    
    return true;
}

String WorldTimeApiAdapter::formatTimestamp(const String& rawDateTime) {
    // Convert "2024-08-03T01:02:59.316944-03:00" to "2024-08-02T05:03:15Z"
    String formattedTime = rawDateTime.substring(0, 19); // Take first 19 characters
    formattedTime += "Z"; // Add Z suffix
    return formattedTime;
} 