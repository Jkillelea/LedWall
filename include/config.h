#ifndef LEDWALL_CONFIG_H_
#define LEDWALL_CONFIG_H_

// WiFi
#define WIFI_SSID "HumiditySucks24"
#define WIFI_PASS "freezingpoint!"

// OTA params
#define OTA_HOSTNAME "ledwall" // .local
#define OTA_PORT (8266)


// LED strip params
#define LEDS_PER_STRIP      (46)
#define LED_BASE_BRIGHTNESS (50)

#define NUM_STRIPS (6)
#define TOTAL_LEDS (NUM_STRIPS * LEDS_PER_STRIP)

#endif // LEDWALL_CONFIG_H_
