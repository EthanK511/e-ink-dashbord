#include <GxEPD2_BW.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <TimeLib.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

// Display init
GxEPD2_BW<GxEPD2_266c, GxEPD2_266c::HEIGHT> display(GxEPD2_266c(/*CS*/5, /*DC*/17, /*RST*/16, /*BUSY*/4));

// Wi-Fi credentials
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

// Weather API (replace with your city and API key)
const char* weather_url = "http://api.openweathermap.org/data/2.5/weather?q=City&appid=YOUR_API_KEY&units=metric";

// NTP
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 0, 60000);

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);
  Serial.println("WiFi connected");

  timeClient.begin();
  timeClient.update();

  display.init();
  display.setRotation(1);
  display.setTextColor(GxEPD_BLACK);
}

void loop() {
  timeClient.update();
  String currentTime = timeClient.getFormattedTime();

  // Fetch weather
  HTTPClient http;
  http.begin(weather_url);
  int httpCode = http.GET();
  String payload;
  if(httpCode > 0) payload = http.getString();
  http.end();

  // TODO: Parse JSON for temperature, condition, etc.

  // Render display
  display.firstPage();
  do {
    display.fillScreen(GxEPD_WHITE);
    display.setCursor(10, 30);
    display.setFont(&FreeSansBold12pt7b);
    display.print(currentTime);
    // TODO: Draw weather data
  } while (display.nextPage());

  // Sleep 15 minutes to save power
  esp_sleep_enable_timer_wakeup(15 * 60 * 1000000ULL);
  esp_deep_sleep_start();
}
