#ifndef TEXTE_H
#define TEXTE_H

#include <Arduino.h>
#define TXT_DISCONNECT "Odpoj"
#define TXT_SOFT_AP_DISCONNECT "Soft AP odpoj"
#define TXT_SET_WLAN_MODE "Nastav WLAN Mode"
#define TXT_CONNECTING_TO "Pripajam k "
#define TXT_CONNECTING_ESTABLISH " zalozit"
#define TXT_NO_CONNECTION "Bez pripojenia! \nStarte Access-Point."
#define TXT_IP_ADDRESS "IP-Adresa = "

#define TXT_RECONNECTED "Znova pripojene"
#define TXT_NOT_CONNECTED "Nepripojene"
#define TXT_CONNECT_TO_AP "Pripoj AP cez SSID "
#define TXT_CONFIG_IP "Nastav IP: "

#define TXT_OK "OK"
extern const char* months[];
//day and month names
const char* const PROGMEM days[] = { "Nedela", "Pondelok", "Utorok", "Streda", "Stvrtok", "Piatok", "Sobota" };
const char* const PROGMEM days_short[] = { "NE", "PO", "UI", "ST", "SR", "PI", "SO" };


#endif  // TEXTE_H