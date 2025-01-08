// Instances of the Preferences class for storing preferences
#include <Arduino.h>
#include <Preferences.h> 
#include "fonts.h"
extern Preferences pref;    // General instance for using the preferences
extern Preferences sender;  // Additional instance, possibly for special configurations

// Structure for storing station data
typedef struct {
  char url[150];    // URL of the station's stream
  char name[32];    // Names of Stations
  uint8_t enabled;  // Flag for activating the station (1 = active, 0 = inactive)
} Station;

// Konstanten
#define STATIONS 30           // Number of stations in the list
#define MINUTES_PER_DAY 1440  // Number of minutes in a day (24 hours * 60 minutes)

// Global variables
extern Station stationlist[STATIONS];  // List of available stations

// Variables for storing configuration data
extern String ssid;                // SSID for the WiFi connection
extern String pkey;                // Password for the WiFi connection
extern String ntp;                // URL of the NTP server for time synchronization
extern uint8_t curStation;           // Index of the currently selected station in the stationlist
extern uint8_t curGain;           // Current volume (gain)
extern uint8_t snoozeTime;         // Snooze time in minutes
extern uint16_t alarm1;           // First alarm time (6:30 a.m. in minutes after midnight)
extern uint8_t alarmday1;         // Valid days of the week for the first alarm (Example: 00111110 = Monday to Friday)
extern uint16_t alarm2;           // Second alarm time (8:00 a.m. in minutes after midnight)
extern uint8_t alarmday2;           // Valid days of the week for the second alarm (Example: 01000001 = Sunday and Saturday)
extern uint8_t actStation;        // Index of the current station in the list, used for streaming
extern uint8_t bright;           // Brightness in percent. 0 means that the LDR controls the brightness

// More global variables
//extern uint32_t lastchange = 1;    // Time of last selection change
extern uint8_t snoozeWait;      // Minutes remaining for snooze mode
extern uint16_t alarmtime;      // Next relevant alarm time
extern uint8_t alarmday;        // Day of the week for the next relevant alarm or 8 means alarm disabled
extern char title[64];              // Character array to store the metadata message
extern bool newTitle;       // Flag to signal a new title
extern uint32_t tick;         // Last value of the tick counter for triggering time-controlled events
extern uint32_t discon;         // Tick ​​counter value for calculating separation time
extern uint16_t minutes;      // Current number of minutes since midnight
extern uint8_t weekday;             // Current day of the week
extern struct tm ti;                // Time structure with the current time
extern int16_t lastldr;             // Last value from LDR sensor to detect changes
extern uint32_t start_conf;         // Time of entering the configuration screen
extern boolean connected;           // Flag to signal an active connection
extern boolean radio;       // Flag to signal radio playback
extern boolean clockmode;    // Flag to signal that the time is displayed on the screen
extern boolean configpage;  // Flag to signal that the configuration page is displayed
extern boolean alarmpage;   // Flag to signal that the alarm setting page is displayed



// Text alignment definitions
#define ALIGNLEFT 0    // Text alignment: left aligned
#define ALIGNCENTER 1  // Text alignment: center aligned
#define ALIGNRIGHT 2   // Text alignment: right aligned

// Font definitions
#define FNT9 &AT_Standard9pt7b  // Font 9pt
#define FNT12 &AT_Bold12pt7b    // Font 12pt
#define LS 23                   // Line spacing

// WiFi configuration
#define NETWORK_NAME "radioweckerlanname"  // Defines the hostname
#define AP_NAME "radioweckerconf"          // Defines the SSID-Name of Soft-AP

// Time zone definition for Central Europe (Germany, France, Italy, Spain, etc.)
#define TIME_ZONE "CET-1CEST,M3.5.0/02,M10.5.0/03"  // Time zone for Central Europe with summer time regulation

// OTA-configuration
#define OTA_HOSTNAME "radiowecker"   // Defines the hostname for OTA Update
#define OTA_PASSWORD "weckerupdate"  // Defines the Password for OTA Update