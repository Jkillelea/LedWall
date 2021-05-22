#pragma once

#define SERVER_PATH_MAX (64)
typedef struct
{
    char path[SERVER_PATH_MAX];
    void (*callback)(void);
} ServerActions_t;


void handleRoot(void);
void handleHue(void);
void handleSat(void);
void handleNotFound(void);
