#ifndef LOGGING_H_
#define LOGGING_H_

#include <ESP8266WiFi.h>

#include "config.h"

typedef enum
{
    Log_Trace   = 0,
    Log_Debug   = 1,
    Log_Info    = 2,
    Log_Message = 3, // should be the lowest level shown to the user by default
    Log_Warning = 4,
    Log_Error   = 5,
    Log_Fatal   = 6,
} LOG_Level;

#define LOGGER_LEVEL (Log_Debug)

#define LOGGER_BEGIN do { if (DO_LOG) {Serial.begin(SERIAL_BAUD);}} while (0)

#define LOG(level, ...) if (level >= LOGGER_LEVEL) { \
    Serial.print("["); \
    Serial.print(__FILE__); \
    Serial.print(": "); \
    Serial.print(__LINE__); \
    Serial.print("]: "); \
    Serial.println(__VA_ARGS__); \
}

#if 0

#define LOGGER_BEGIN if (true) {\
\
}

#define LOG(level, ...) do {\
if (level >= LOGGER_LEVEL) { \
}

#endif


#define LOG_TRACE(...) LOG(Log_Trace, __VA_ARGS__)
#define LOG_DEBUG(...) LOG(Log_Debug, __VA_ARGS__)
#define LOG_INFO(...)  LOG(Log_Info, __VA_ARGS__)
#define LOG_MSG(...)   LOG(Log_Message, __VA_ARGS__)
#define LOG_WARN(...)  LOG(Log_Warning, __VA_ARGS__)
#define LOG_ERROR(...) LOG(Log_Error, __VA_ARGS__)
#define LOG_FATAL(...) LOG(Log_Fatal, __VA_ARGS__)

#endif // LOGGING_H_
