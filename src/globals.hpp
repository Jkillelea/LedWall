#pragma once

#include <Adafruit_NeoPixel.h>
#include <ESP8266WebServer.h>
#include "renderer.hpp"
#include "config.h"

extern Adafruit_NeoPixel g_LED_STRIPS[NUM_STRIPS];
extern ESP8266WebServer g_WebServer;
extern std::shared_ptr<Renderer<NUM_STRIPS, LEDS_PER_STRIP>> g_LedRenderer;

