#include "serveractions.hpp"
#include "logging.h"
#include "globals.hpp"

/* server callbacks */
void handleRoot(void)
{
    String text = String("Hello, runtime is ") + millis() + " milliseconds\r\n\r\n";
    g_WebServer.send(200, "text/plain", text);
}

void handleHue(void)
{
    LOG(Log_Debug, "Handling hue");
    uint16_t h = 0;
    if (g_WebServer.hasArg("hue"))
    {
        LOG(Log_Debug, "Have arg");
        String hueString = g_WebServer.arg("hue");
        sscanf(hueString.c_str(), "%hd", &h);
        LOG(Log_Debug, h);
        g_LedRenderer->setHue(h);
    }

    g_WebServer.send(200, "text/plain", String(h));
}

void handleNotFound(void)
{
    g_WebServer.send(404, "text/plain", "not found\r\n\r\n");
    LOG(Log_Error, "Server could not find requested path!");
    switch (g_WebServer.method())
    {
        case HTTP_ANY:
            LOG(Log_Error, "Method: HTTP_ANY");
            break;
        case HTTP_GET:
            LOG(Log_Error, "Method: HTTP_GET");
            break;
        case HTTP_HEAD:
            LOG(Log_Error, "Method: HTTP_HEAD");
            break;
        case HTTP_POST:
            LOG(Log_Error, "Method: HTTP_POST");
            break;
        case HTTP_PUT:
            LOG(Log_Error, "Method: HTTP_PUT");
            break;
        case HTTP_PATCH:
            LOG(Log_Error, "Method: HTTP_PATCH");
            break;
        case HTTP_DELETE:
            LOG(Log_Error, "Method: HTTP_DELETE");
            break;
        case HTTP_OPTIONS:
            LOG(Log_Error, "Method: HTTP_OPTIONS");
            break;
        default:
            LOG(Log_Error, "Method: UNKNOWN");
            break;
    }

    LOG(Log_Error, "== Args ==");
    for (int i = 0; i < g_WebServer.args(); i++)
    {
        String line = g_WebServer.argName(i) + " : " + g_WebServer.arg(i);
        LOG(Log_Error, line);
    }

    LOG(Log_Error, "== Headers ==");
    for (int i = 0; i < g_WebServer.headers(); i++)
    {
        String line = g_WebServer.headerName(i) + " : " + g_WebServer.header(i);
        LOG(Log_Error, line);
    }
}
