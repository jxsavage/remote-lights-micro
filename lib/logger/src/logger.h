#ifndef SERIAL_LOGGER_H
#define SERIAL_LOGGER_H
#include <ArduinoJson.h>
inline void REMOTE_LOG(JsonArray message) {
  serializeJson(message, Serial);
  Serial.print('\n');
  // Serial.send_now();
  Serial.flush();
}
template<typename T, typename ...Ts> inline void REMOTE_LOG(JsonArray logger, T value, Ts... args) {
    logger.add(value);
    REMOTE_LOG(logger, args...);
}
template<typename T, typename ...Ts> inline void REMOTE_LOG(uint8_t msgType, T value, Ts... args) {
    StaticJsonDocument<JSON_ARRAY_SIZE(10)> loggerDoc;
    JsonArray logger = loggerDoc.to<JsonArray>();
    logger.add(0);
    logger.add(msgType);
    logger.add(value);
    REMOTE_LOG(logger, args...);
}
#endif