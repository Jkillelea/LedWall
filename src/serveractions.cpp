#include "serveractions.hpp"
#include "logging.h"
#include "globals.hpp"
#include <LittleFS.h>

/* server callbacks */
void handleRoot(void)
{
    File f = LittleFS.open("/index.html", "r");
    String text = f.readString();
    g_WebServer.send(200, "text/html", text);
}

void handleHue(void)
{
    LOG_DEBUG("Handling hue");
    uint16_t h = 0;

    if (g_WebServer.hasArg("hue"))
    {
        LOG_DEBUG("Have arg");
        String hueString = g_WebServer.arg("hue");
        sscanf(hueString.c_str(), "%hd", &h);
        LOG_DEBUG(h);
        g_LedRenderer->setHue(h);
    }

    g_WebServer.send(200, "text/plain", String(h));
}

void handleSat(void)
{
    LOG_DEBUG("Handling sat");
    int s = 0;

    if (g_WebServer.hasArg("sat"))
    {
        LOG_DEBUG("Have arg");
        String satString = g_WebServer.arg("sat");
        sscanf(satString.c_str(), "%d", &s);
        LOG_DEBUG(s);
        g_LedRenderer->setSat(s);
    }

    g_WebServer.send(200, "text/plain", String(s));
}


void handleNotFound(void)
{
    // If it's a file that exists
    if (LittleFS.exists(g_WebServer.uri()))
    {
        File f = LittleFS.open(g_WebServer.uri(), "r");
        g_WebServer.send(200, "text/plain", f.readString());
        return;
    }

    g_WebServer.send(404, "text/plain", "not found\r\n\r\n");
    LOG_ERROR("Server could not find requested path!");
    LOG_ERROR(g_WebServer.uri());

    switch (g_WebServer.method())
    {
        case HTTP_ANY:
            LOG_ERROR("Method: HTTP_ANY");
            break;
        case HTTP_GET:
            LOG_ERROR("Method: HTTP_GET");
            break;
        case HTTP_HEAD:
            LOG_ERROR("Method: HTTP_HEAD");
            break;
        case HTTP_POST:
            LOG_ERROR("Method: HTTP_POST");
            break;
        case HTTP_PUT:
            LOG_ERROR("Method: HTTP_PUT");
            break;
        case HTTP_PATCH:
            LOG_ERROR("Method: HTTP_PATCH");
            break;
        case HTTP_DELETE:
            LOG_ERROR("Method: HTTP_DELETE");
            break;
        case HTTP_OPTIONS:
            LOG_ERROR("Method: HTTP_OPTIONS");
            break;
        default:
            LOG_ERROR("Method: UNKNOWN");
            break;
    }

    LOG_ERROR("== Args ==");
    for (int i = 0; i < g_WebServer.args(); i++)
    {
        String line = g_WebServer.argName(i) + " : " + g_WebServer.arg(i);
        LOG_ERROR(line);
    }

    LOG_ERROR("== Headers ==");
    for (int i = 0; i < g_WebServer.headers(); i++)
    {
        String line = g_WebServer.headerName(i) + " : " + g_WebServer.header(i);
        LOG_ERROR(line);
    }
}
