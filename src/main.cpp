#include <Arduino.h>


// 2.attemp to modify to larger metadata field on display and translate german to english
// translated to english 2.10.2024
#define RADIOVERSION "v3.0.5";
#include "audio.h"
#include "00_librarys.h"      //Load all libraries 
#include "00_pin_settings.h"  //Set required pins
#include "00_settings.h"      //N Setting
#include "index.h"            //N Index page
#include "00_texte.h"        //Strings (texts)
uint8_t actStation = 0;
uint8_t curGain = 50;
uint8_t curStation = 0;
uint8_t bright = 0;
uint16_t minutes = 0;
uint8_t weekday = 0;
struct tm ti;
char title[64] = "";
int16_t lastldr = 0;
uint32_t start_conf = 0;
boolean connected = false;
boolean alarmpage = false;
uint8_t snoozeWait = 0;
uint32_t tick = 0;
uint32_t discon = 0;
boolean radio = false;
boolean clockmode = false;
boolean newTitle = false;
String ssid = "";                // SSID for the WiFi connection
String pkey = "";                // Password for the WiFi connection
String ntp = "de.pool.ntp.org";  // URL of the NTP server for time synchronization

Preferences pref;
Preferences sender;
//predefined function from modul tft_display.ino
void displayMessage(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const char* text, uint8_t align = ALIGNLEFT, boolean big = false, uint16_t fc = ILI9341_WHITE, uint16_t bg = ILI9341_BLACK, uint8_t lines = 1);

// Calculates the date and time for the next expected alarm
// Sets `alarmtime` to the time of day and `alarmday` to the day of the week for the alarm
// or to 8 if there is no alarm



// Initialyzing the System
void setup() {
  // Start Serial communication with Baudrate 115200
  Serial.begin(115200);
  Serial.println("Load preferences");  // Debug output: "Load Settings"

  title[0] = 0;  // Set the first Letter of Titel for 0 (Clear Titel)

  // Load the saved preferences from the EEPROM of the ESP32
  // The preferences are stored in two topics: "radio" and "station list"
  // Both topics are loaded here
  pref.begin("radio", false);
  sender.begin("senderlist", false);

  // Retrieve values ​​from saved preferences, if any
  if (pref.isKey("ssid")) ssid = pref.getString("ssid");  // SSID for Lan connection
  if (pref.isKey("pkey")) pkey = pref.getString("pkey");  // Password for WLAN-connection
  if (pref.isKey("ntp")) ntp = pref.getString("ntp");     // NTP-Server-URL to get Time

  uint8_t curGain = 50;                                              // Standard value for Loudness
  if (pref.isKey("gain")) curGain = pref.getUShort("gain");  // Get the Volume value

  
  uint8_t bright = 80;                                                  // Standard value for Brightness in minutes
  if (pref.isKey("bright")) bright = pref.getUShort("bright");  // Get the Brightness

  
  
  uint8_t curStation = 0;  // Standard value for actual station
  // Set the current station to the saved value, if available
  if (pref.isKey("station")) curStation = pref.getUShort("station");
  // Check whether the current station is within the valid range
  if (curStation >= STATIONS) curStation = 0;

  uint8_t actStation = curStation;  // Set the active station to the current station

  // Debug output of current station, volume, SSID and NTP server
  Serial.printf("station %i, gain %i, ssid %s, ntp %s\n", curStation, curGain, ssid.c_str(), ntp.c_str());

  // Execute the setup functions for audio, display and channel list
  setup_audio();       // Initialize the Audio-Streams
  setup_display();     // Initialize the Display-Interface
  setup_senderList();  // Load Sender List from Preferences
  setGain(0);          // Set the actual Laudness

  // Try to connect to Wi-Fi and see the progress on the display
  displayClear();                                                                                          // Clear Screen
  displayMessage(5, 10, 310, 30, TXT_CONNECTING_TO, ALIGNCENTER, true, ILI9341_YELLOW, ILI9341_BLACK, 1);  // Display "Connecting to"
  displayMessage(5, 50, 310, 30, ssid.c_str(), ALIGNCENTER, true, ILI9341_GREEN, ILI9341_BLACK, 1);        // Show SSID on the Display
  Serial.println("Connect WiFi");                                                                          // Debug output: "Connecting to WiFi"

  // Try connecting to WiFi
  connected = initWiFi(ssid, pkey);

  // If the connection was successful
  if (connected) {
    // Configure the real time clock with the time zone and NTP server
    configTzTime(TIME_ZONE, ntp.c_str());

    // Display the date and time as well as the name of the station
    delay(500);  // Short delay for display
    // Fill the time structure ti, minutes and day of the week with the current time
    getLocalTime(&ti);
    minutes = ti.tm_hour * 60 + ti.tm_min;  // Calculate minutes since midnight
    weekday = ti.tm_wday;                   // Week day (0 = Sunday, 1 = Monday, ...)

    Serial.println("Start");  // Debug-Output: "Start"

    // If the alarm is activated, calculate the date and time for the next alarm
    //if (pref.isKey("alarmon") && pref.getBool("alarmon")) findNextAlarm();

    // Show time and next alarm on the display
    //showClock();
  } else {  //If the connection was not successful
    // A connection could not be established. A corresponding message will appear on the display
    displayClear();                                                                                          // Clear Display
    displayMessage(5, 10, 310, 30, TXT_NOT_CONNECTED, ALIGNCENTER, true, ILI9341_RED, ILI9341_BLACK, 1);     // Show "not connected" on the Display
    displayMessage(5, 50, 310, 30, TXT_CONNECT_TO_AP, ALIGNCENTER, true, ILI9341_WHITE, ILI9341_BLACK, 1);  // Show "Connecting to AP" on the Display
    displayMessage(5, 100, 310, 30, AP_NAME, ALIGNCENTER, true, ILI9341_GREEN, ILI9341_BLACK, 1);            // Show "Ap Name" on the Display
    displayMessage(5, 150, 310, 30, TXT_CONFIG_IP, ALIGNCENTER, true, ILI9341_WHITE, ILI9341_BLACK, 1);     // Show "IP configuration" on the Display
    displayMessage(5, 200, 310, 30, "192.168.4.1", ALIGNCENTER, true, ILI9341_GREEN, ILI9341_BLACK, 1);     // Show "IP " on the Display

    // Note the current timestamp to enable a retry later after 5 minutes
    discon = millis();
  }

  Serial.println("Start webserver");  // Debug-Output: "Starte Webserver"

  // Initialize Webserver and the OTA-Update
  setup_webserver();  // Initialize Webserver
  setup_ota();        // Initialize OTA-Update

  // Remember the current timestamp for timed events
  tick = millis();
  start_conf = 0;  // Set the start value for the Configuration
}

void loop() {
  long rssi = WiFi.RSSI();  // Get the signal strength (RSSI) level of the WiFi

  // Review and process possible OTA updates
  ArduinoOTA.handle();

  // Process HTTP requests
  webserver_loop();

  // Check touch events
  touch_loop();

  // After 10 seconds in configuration mode, switch back to clock mode
  if (!clockmode && ((millis() - start_conf) > 10000)) {
    showClock();       // Show Time
    clockmode = true;  // Set mode to Hours
  }

  // Show metadata when the radio is active and we are in clock mode
  if (newTitle && radio && clockmode) {
    showTitle();       // Show Titel of actual radio Station
    newTitle = false;  // Reset the flag
  }

  // Detect a connection loss
  if (connected && (WiFi.status() != WL_CONNECTED)) {
    connected = false;  // Set connection to false
    discon = millis();  // Note the current timestamp for an automatic retry
    displayClear();     // Clear Display
    // Display a message that the connection is lost
    displayMessage(5, 10, 310, 30, TXT_NOT_CONNECTED, ALIGNCENTER, true, ILI9341_RED, ILI9341_BLACK, 1);
  }

  // Recognize reconnection
  if (!connected && (WiFi.status() == WL_CONNECTED)) {
    connected = true;  // Set connection to true
    displayClear();    // Clear Display
    // Display a message that the connection has been restored
    displayMessage(5, 10, 310, 30, TXT_RECONNECTED, ALIGNCENTER, true, ILI9341_GREEN, ILI9341_BLACK, 1);
  }

  // When the radio is on, get new stream data
  if (connected && radio) {
    audio_loop();  // Process audio data
  }

  // When brightness is set to 0, the brightness is controlled by the ambient light
  if (bright == 0) {
    int16_t tmp = analogRead(LDR);  // Read the value from the Light sensor (LDR)
    int16_t diff = lastldr - tmp;   // Calculate the difference from the previous value
    diff = abs(diff);               // Take the absolute value of the difference

    // Set the backlight when the clock is displayed and a significant change in ambient light is detected
    if (clockmode && (diff > 50)) {
      setBGLight(bright);                                  // Set the backlight
      Serial.printf("ldr %i letzter %i\n", tmp, lastldr);  // Debug-Output for actual and the last LDR value
      lastldr = tmp;                                       // Note the current LDR value for the next measurement 
    }
  }

  // Timed event: Display refresh every 60 seconds
  if ((millis() - tick) > 60000) {
    tick = millis() - ti.tm_sec * 1000;  // Time correction (Kingherold ISSUE: Time is not corrected)
    // Get the date and time
    if (connected && getLocalTime(&ti)) {
      minutes = ti.tm_hour * 60 + ti.tm_min;  // Calculate minutes since midnight
      weekday = ti.tm_wday;                   // Week day (0 = Sonday, 1 = Monday, ...)
    }
    // Set the backlight when the clock is displayed
    if (connected && clockmode) {
      setBGLight(bright);  // Set the Backlight
      displayDateTime();   // Show Date and Time
    }
    // If snooze mode is activated, count down the snooze counter and turn off the radio when the counter reaches 0
    // If an alarm is activated, check the day and time
    
  }

  // Reboot if the device has been disconnected for more than 5 minutes
  if (!connected && ((millis() - discon) > 300000)) ESP.restart();
}