#ifndef LOGGING_H_
#define LOGGING_H_


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

#define DO_LOG (true)
#define LOGGER_LEVEL (Log_Debug)

#define LOGGER_BEGIN do { if (DO_LOG) {Serial.begin(115200);}} while (0)

#define LOG(level, ...) do {\
if (DO_LOG && (level >= LOGGER_LEVEL)) { \
    Serial.print("["); \
    Serial.print(__LINE__); \
    Serial.print(": "); \
    Serial.print(millis()); \
    Serial.print("]: "); \
    Serial.println(__VA_ARGS__); \
}} while (0)

#endif // LOGGING_H_
