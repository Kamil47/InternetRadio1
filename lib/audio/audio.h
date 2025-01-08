#include <ArduinoOTA.h>
#ifndef AUDIO_H
#define AUDIO_H
#include <Arduino.h>
#define RADIOVERSION "v3.0.5";
boolean initWiFi(String ssid, String pkey);
extern const unsigned char ziffern[11][500] PROGMEM;
void showProgress(uint32_t prc);
void displayMessage(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const char *msg, uint8_t align, boolean clear, uint16_t fg, uint16_t bg, uint8_t size);
//void toggleRadio();
void showRadio();
void findNextAlarm();
void showNextAlarm();
void setBGLight(uint8_t bright);
void displayDateTime();
void audio_loop();
void showTitle();
void showClock();
void touch_loop();
void webserver_loop();
void setup_ota();
void setup_webserver();
void setup_audio();
void setup_display();
void setup_senderList();
void displayClear();
void setGain(float gain);
void beforeStation();
void nextStation();
void changeStation();
void showStation();
void showGainMain();
void showGain();
void showVolume();
void stopPlaying();
//void startUrl(String url);
void setStationData();
void handleRoot();
void ota_onStart();
void ota_onEnd();
void ota_onProgress(unsigned int progress, unsigned int total);
void toggleRadio(boolean off);
void ota_onError(ota_error_t error);
void reorder(uint8_t from, uint8_t to);
void startSnooze();
void showRadio();
void showRadioPage();
void toggleAlarm();
void setGain(float gain);
void setGainValue(uint16_t value, const char* sliderPage);
bool startUrl(String url);
void restore();
#endif // AUDIO_H