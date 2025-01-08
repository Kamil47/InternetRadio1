// Number of standard stations in the list
// Included a few stations from Slovakia
#include <Arduino.h>
#include "audio.h"
#include "00_settings.h"
#define DEFAULTSTATIONS 26
Station stationlist[STATIONS];

// Definition of a set of tested standard stations, stored in flash memory (PROGMEM)
Station defstations[DEFAULTSTATIONS] PROGMEM = {
  { "http://live.slovakradio.sk:8000/Slovensko_256.mp3", "SRO1 BRATISLAVA", 1}, 
  { "http://stream.radiovlna.sk:8000/rock-hi.mp3", "RADIO VLNA", 1},
  { "http://stream.expres.sk/128.mp3", "RADIO EXPRES", 1},
  { "http://stream.funradio.sk:8000/fun192.mp3", "FUN RADIO", 1},
  { "http://live.slovakradio.sk:8000/Devin_256.mp3", "DEVIN", 1},
  { "http://play.radio7.sk/128", "PLAY RADIO", 1}, 
  { "https://c10.radioboss.fm:18122/stream", "RADIOBOSS", 1},
  { "http://stream.funradio.sk:8000/dance128.mp3", "FAN RADIO DANCE", 1},
  { "http://stream.sepia.sk:8000/viva128.mp3", "SEPIA", 1}, 
  { "http://live.slovakradio.sk:8000/RSI_128.mp3", "INTERNATIONAL SK", 1},
  { "http://646.cloudrad.io:8314", "DJ Perry", 1 },
  { "http://1a-rock.radionetz.de:8000/1a-rock.mp3", "1A Rock", 1 },
  { "http://1a-ndw.radionetz.de/1a-ndw.mp3", "1A NDW", 1 },
  { "http://mdr-284280-1.cast.mdr.de/mdr/284280/1/mp3/low/stream.mp3", "MDR", 1 },
  { "http://icecast.ndr.de/ndr/njoy/live/mp3/128/stream.mp3", "N-JOY", 1 },
  { "http://rnrw.cast.addradio.de/rnrw-0182/deinrock/low/stream.mp3", "NRW Rockradio", 1 },
  { "http://rnrw.cast.addradio.de/rnrw-0182/dein90er/low/stream.mp3", "NRW 90er", 1 },
  { "http://rt1.radio/tu6h1", "RT1 Rock", 1 },
  { "http://stream.streambase.ch/vrock/mp3-192/", "Virgin Radio", 1 },
};

// Populates the station list from the saved preferences. If an entry does not exist, the default list is used
void setup_senderList() {
  char nkey[4];  // Key for the Names
  char ukey[4];  // Key for the URL
  char fkey[4];  // Activation status key

  // Loop over all possible stations
  for (uint8_t i = 0; i < STATIONS; i++) {
    // Generate the keys for the current index station
    sprintf(nkey, "n%i", i);
    sprintf(ukey, "u%i", i);
    sprintf(fkey, "f%i", i);

    if (i < DEFAULTSTATIONS) {
      //If the index is within the default stations, load the data from preferences or use default values
      strlcpy(stationlist[i].name, sender.getString(nkey, defstations[i].name).c_str(), 32);
      strlcpy(stationlist[i].url, sender.getString(ukey, defstations[i].url).c_str(), 150);
      stationlist[i].enabled = sender.getUChar(fkey, defstations[i].enabled);
    } else {
      // If the index is outside of the default stations, load the data from preferences or use empty defaults
      strlcpy(stationlist[i].name, sender.getString(nkey, nkey).c_str(), 32);
      strlcpy(stationlist[i].url, sender.getString(ukey, "").c_str(), 150);
      stationlist[i].enabled = sender.getUChar(fkey, 0);
    }
  }
}

// Fills the station list with the default stations and saves them in the preferences
void restore() {
  char nkey[4];  // Key for the Names
  char ukey[4];  // Key for the URL
  char fkey[4];  // Activation status key

  // Loop over all possible stations
  for (uint8_t i = 0; i < STATIONS; i++) {
    // Delete the previous entries in the preferences
    sender.clear();

    // Create the keys for the current index
    sprintf(nkey, "n%i", i);
    sprintf(ukey, "u%i", i);
    sprintf(fkey, "f%i", i);

    if (i < DEFAULTSTATIONS) {
      // If the index is within the standard stations, use the default values
      strlcpy(stationlist[i].name, defstations[i].name, 32);
      sender.putString(nkey, defstations[i].name);
      strlcpy(stationlist[i].url, defstations[i].url, 150);
      sender.putString(ukey, defstations[i].url);
      stationlist[i].enabled = defstations[i].enabled;
      sender.putUChar(fkey, defstations[i].enabled);
    } else {
      // If the index is outside the standard stations, store empty values
      strlcpy(stationlist[i].name, nkey, 32);
      sender.putString(nkey, nkey);
      strlcpy(stationlist[i].url, "", 150);
      sender.putString(ukey, "");
      stationlist[i].enabled = 0;
      sender.putUChar(fkey, 0);
    }
  }
}

// Saves the station list in the preferences
void saveList() {
  char nkey[4];  // Key for the Names
  char ukey[4];  // Key for the URL
  char fkey[4];  // Activation status key

  // Loop over all possible stations
  for (uint8_t i = 0; i < STATIONS; i++) {
    // Create the keys for the current index
    sprintf(nkey, "n%i", i);
    sprintf(ukey, "u%i", i);
    sprintf(fkey, "f%i", i);

    // Save the station data in the preferences
    sender.putString(nkey, stationlist[i].name);
    sender.putString(ukey, stationlist[i].url);
    sender.putUChar(fkey, stationlist[i].enabled);
  }
}

// Moves a station within the station list from the old position to the new position
void reorder(uint8_t oldpos, uint8_t newpos) {
  Station temp;  // Temporary variable to save the station while moving

  // Check whether the old and new positions are within the valid limits
  if ((oldpos < STATIONS) && (newpos < STATIONS)) {
    if (oldpos > newpos) {
      // Move up in the list
      memcpy(&temp, &stationlist[oldpos], sizeof(Station));  // Save the station temporarily
      for (uint8_t i = oldpos; i > newpos; i--) {
        memcpy(&stationlist[i], &stationlist[i - 1], sizeof(Station));  // Move the stations up
      }
      memcpy(&stationlist[newpos], &temp, sizeof(Station));  // Put the station in the new position
    } else {
      // Move down the list
      memcpy(&temp, &stationlist[oldpos], sizeof(Station));  // Save the station temporarily
      for (uint8_t i = oldpos; i < newpos; i++) {
        memcpy(&stationlist[i], &stationlist[i + 1], sizeof(Station));  // Move the stations up
      }
      memcpy(&stationlist[newpos], &temp, sizeof(Station));  // Put the station in the new position
    }
  }
}