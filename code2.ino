#include <GxEPD2_BW.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <TimeLib.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

// ------------------- DISPLAY SETUP -------------------
GxEPD2_BW<GxEPD2_266c, GxEPD2_266c::HEIGHT> display(GxEPD2_266c(/*CS*/5, /*DC*/17, /*RST*/16, /*BUSY*/4));

// ------------------- WIFI SETUP -------------------
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// ------------------- WEATHER API -------------------
const char* weather_url = "http://api.openweathermap.org/data/2.5/weather?q=City&appid=YOUR_API_KEY&units=metric";

// ------------------- CALENDAR (Google ICS public feed) -------------------
const char* calendar_url = "YOUR_PUBLIC_ICS_FEED_URL";

// ------------------- NTP TIME -------------------
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 0, 60000);

void setup() {
  Serial.begin(115200);

  // Connect Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");

  // Start NTP
  timeClient.begin();
  timeClient.update();

  // Initialize display
  display.init();
  display.setRotation(1);
  display.setTextColor(GxEPD_BLACK);
}

void loop() {
  // --- UPDATE TIME ---
  timeClient.update();
  String currentTime = timeClient.getFormattedTime();

  // --- FETCH WEATHER ---
  String weatherText = "Loading weather...";
  HTTPClient http;
  http.begin(weather_url);
  int httpCode = http.GET();
  if(httpCode > 0){
    String payload = http.getString();
    StaticJsonDocument<512> doc;
    deserializeJson(doc, payload);
    float temp = doc["main"]["temp"];
    String desc = doc["weather"][0]["main"].as<String>();
    weatherText = String(temp) + "°C, " + desc;
  }
  http.end();

  // --- FETCH CALENDAR EVENTS (simple ICS parsing) ---
  String calendarText = "No events";
  http.begin(calendar_url);
  httpCode = http.GET();
  if(httpCode > 0){
    String ics = http.getString();
    int start = ics.indexOf("SUMMARY:");
    if(start > 0){
      int end = ics.indexOf("\\n", start);
      calendarText = ics.substring(start + 8, end);
    }
  }
  http.end();

  // --- DRAW TO DISPLAY ---
  display.firstPage();
  do {
    display.fillScreen(GxEPD_WHITE);
    
    // TIME
    display.setCursor(10, 30);
    display.setFont(&FreeSansBold12pt7b);
    display.print(currentTime);
    
    // WEATHER
    display.setCursor(10, 70);
    display.setFont(&FreeSans9pt7b);
    display.print(weatherText);
    
    // CALENDAR
    display.setCursor(10, 110);
    display.setFont(&FreeSans9pt7b);
    display.print("Next Event:");
    display.setCursor(10, 130);
    display.print(calendarText);

  } while (display.nextPage());

  // --- DEEP SLEEP (refresh every 15 minutes) ---
  esp_sleep_enable_timer_wakeup(15 * 60 * 1000000ULL);
  esp_deep_sleep_start();
}
