// Library for WiFi and Over-the-Air (OTA) Updates
#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoOTA.h>

// TFT & Touchscreen Library
#include <Adafruit_GFX.h>           // Library for Graphic Display
#include <Adafruit_ILI9341.h>       // Driver Library for ILI9341 TFT-Display
#include <XPT2046_Touchscreen.h>    // Driver Library for ILI9341 XPT2046 Touchscreen
#include <TouchEvent.h>             // Library to analyse Touchscreen-Events

// ESP32 Library for Flash Memory Setting
#include <Preferences.h>

// Library for installed Webserver
#include <WebServer.h>

// Library for Audio-Functions
// Classes for Audio Processing 
#include <AudioFileSourceICYStream.h>  // Class for input Stream ICY (Internet Radio) Streams
#include <AudioFileSourceBuffer.h>     // Class for Buffering Audio-Daten
#include <AudioGeneratorMP3.h>         // MP3-Decoder Class
#include <AudioOutputI2S.h>            // Class for audio output via I2S (Inter-IC Sound) interface