#include <Arduino.h>
#include "audio.h"
#include <ArduinoOTA.h>
#include "00_settings.h"
#include "00_librarys.h"
#include "tft_color_setting.h"
#include <Adafruit_ILI9341.h>

// Variable to track OTA update progress
uint32_t oldprc;

// OTA update preparation function
void setup_ota() {
  // Set the hostname and password for OTA updates
  ArduinoOTA.setHostname(OTA_HOSTNAME);  // Set the Hostnamen for OTA
  ArduinoOTA.setPassword(OTA_PASSWORD);  // Set the Password for OTA

  // Register callback routines for OTA events
  ArduinoOTA.onStart(ota_onStart);        // Callback when OTA update begins
  ArduinoOTA.onEnd(ota_onEnd);            // Callback when OTA update ends
  ArduinoOTA.onProgress(ota_onProgress);  // Callback to display progress
  ArduinoOTA.onError(ota_onError);        // Callback in case of failure during OTA update

  // Start OTA processing
  ArduinoOTA.begin();  // OTA update begins
}

// Callback function that will be called when the OTA update begins
void ota_onStart() {
  displayClear();   // Clear the Display
  setBGLight(100);  // Sets the backlight to 100% brightness
  oldprc = 0;       // Resets the old progress value
  String type;
  // Determine the type of update (sketch or file system)
  if (ArduinoOTA.getCommand() == U_FLASH)
    type = "sketch";      // Program update
  else                    // U_SPIFFS
    type = "filesystem";  // File system update

  // Show a message on the display that the sketch is updating
  displayMessage(5, 10, 310, 30, "Updating Sketch", ALIGNCENTER, true, ILI9341_YELLOW, ILI9341_BLACK, 1);
  showProgress(0);  // Show Progress (start at 0%)
}

// Callback function called when the OTA update ends
void ota_onEnd() {
  // Displays a message on the display indicating that the update is complete
  displayMessage(5, 120, 310, 30, "Done", ALIGNLEFT, true, ILI9341_GREEN, ILI9341_BLACK, 1);
}

//Callback function called to show OTA update progress 
void ota_onProgress(unsigned int progress, unsigned int total) {
  uint32_t prc = progress / (total / 100);  // Calculate progress in percent
  char tmp[20];
  // Wenn der Fortschritt gestiegen ist, aktualisiere die Fortschrittsanzeige
  if (prc > oldprc) {
    showProgress(prc);  // Show progress
    oldprc = prc;       // Save current progress for next review
  }  
}

// Callback function called in case of error during OTA update
void ota_onError(ota_error_t error) {
  char err[80];
  // Formatted error message
  sprintf(err, "Error[%u]: ", error);
  // Determine the specific error message based on the error code
  if (error == OTA_AUTH_ERROR) sprintf(err, "Error[%u]: Auth Failed", error);
  else if (error == OTA_BEGIN_ERROR) sprintf(err, "Error[%u] Begin Failed", error);
  else if (error == OTA_CONNECT_ERROR) sprintf(err, "Error[%u] Connect Failed", error);
  else if (error == OTA_RECEIVE_ERROR) sprintf(err, "Error[%u] Receive Failed", error);
  else if (error == OTA_END_ERROR) sprintf(err, "Error[%u] End Failed", error);

  // Show the error message on the display
  displayMessage(5, 200, 310, 30, err, ALIGNLEFT, false, ILI9341_RED, ILI9341_BLACK, 1);
}