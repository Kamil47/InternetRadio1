//home page and template for options
#include <Arduino.h>
#include "index.h"          //N Index page
#include <ArduinoJson.h>
#include "audio.h"
#include "fonts.h"
#include <WebServer.h>
#include "00_settings.h"      //N Setting
#include "00_texte.h"
WebServer server(80);

// Function prototypes
void sendStations();
void restoreStations();
void restart();
void setAccessData();
void getAccessData();
void getAlarms();

void getStationData();
void setStationData();
void testStation();
void endTest();
void startPlay();
void stopPlay();
void GainSlider();


void beforeStation();
void nextStation();
void getCurrentStatus();
void getInfo();
void saveList();

// Configures the web server and sets up the routes for various HTTP requests
void setup_webserver() {
  // Optional: Enables CORS (Cross-Origin Resource Sharing) for the server
  // server.enableCORS();

  // Defines the route for the root URL pointing to the `handleRoot` function
  server.on("/", handleRoot);

  // Defines the routes for various AJAX commands and associates them with the corresponding functions


  server.on("/cmd/stations", sendStations);            // Gets the list of available stations
  server.on("/cmd/restorestations", restoreStations);  // Restores the saved stations
  server.on("/cmd/restart", restart);                  // Restarts the system
  server.on("/cmd/setaccess", setAccessData);          // Sets the access data
  server.on("/cmd/getaccess", getAccessData);          // Gets the credentials
  
  
  server.on("/cmd/getstation", getStationData);        // Retrieves the data of the current station
  server.on("/cmd/setstation", setStationData);        // Sets the data of the current station
  server.on("/cmd/teststation", testStation);          // Tests the connection to the current station
  server.on("/cmd/endtest", endTest);                  // Exits test mode

  // Player tab: Defines routes for player control commands
  server.on("/cmd/startPlay", startPlay);                // Starts playback
  server.on("/cmd/stopPlay", stopPlay);                  // Stops playback
  server.on("/cmd/GainSlider", GainSlider);              // Sets the value of the volume control
  
  server.on("/cmd/beforeStation", beforeStation);        // Goes to the previous station
  server.on("/cmd/nextStation", nextStation);            // Move to the next station
  server.on("/cmd/getCurrentStatus", getCurrentStatus);  // Gets the current status

  // Info Tab: Defines the route for retrieving system information
  server.on("/cmd/getInfo", getInfo);  //Retrieves general system information 
  // Start Webserver
  server.begin();
}

// Called from the main loop to process incoming HTTP requests
void webserver_loop() {
  server.handleClient();  // Processes all requests sent to the web server
}

// Handles requests to the root URL ("/") of the web server
void handleRoot() {
  bool connected = WiFi.status() == WL_CONNECTED;  // Check if WiFi is connected
  if (connected) {
    // When connected, send the main page
    server.send(200, "text/html", MAIN_page);
  } else {
    // If not connected, send the configuration page
    // Parameters are passed as arguments in the HTML request
    Serial.println("Got config:");
    uint8_t a = server.args();  // Number of arguments in the request
    Serial.print(a);
    Serial.println(" Arguments");
    for (uint8_t i = 0; i < a; i++) Serial.println(server.arg(i));  // Output all arguments

    // Save the SSID in the settings if there is one
    if (server.hasArg("conf_ssid")) {
      pref.putString("ssid", server.arg("conf_ssid"));
      Serial.println(server.arg("conf_ssid"));
    }

    // Save the PKEY in Settings if available
    if (server.hasArg("conf_pkey")) {
      pref.putString("pkey", server.arg("conf_pkey"));
      Serial.println(server.arg("conf_pkey"));
    }

    // If the reset argument is present, restart the system
    if (server.hasArg("reset")) {
      Serial.println("Restart!");
      ESP.restart();
    }

    // Send the configuration page to the client
    server.send(200, "text/html", CONFIG_page);
  }
}

// Handles AJAX commands for "/cmd/stations"
// Sends a list of available stations as HTML options
void sendStations() {
  const char* s;        // Variable for the symbol in front of the station name
  const char* sel;      // Variable for the 'selected' attribute of the current station
  char buf[100];  // Buffer for HTML output

  // Prepare the HTML response
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send(200, "text/html", "");

  // Send all stations as an options list
  for (uint8_t i = 0; i < STATIONS; i++) {
    // Add the 'selected' attribute if the station is current
    sel = "";
    if (i == actStation) sel = "selected";

    // Put a black dot icon in front of the station name if enabled
    s = "&#x2002;";
    if (stationlist[i].enabled) s = "&#x25cf;";

    // Create the entry for the option using the template
    sprintf(buf, OPTION_entry, i, sel, s, stationlist[i].name);
    server.sendContent(buf);  // Send the content of the option
  }
}

// Handles AJAX commands for "/cmd/setaccess"
// Saves access data in the settings
void setAccessData() {
  // The command expects three arguments
  // The access data is saved in the settings

  // Save the SSID if available
  if (server.hasArg("ssid")) {
    ssid = server.arg("ssid");
    pref.putString("ssid", ssid);
  }

  // Save the pre-shared key (PKEY) if available
  if (server.hasArg("pkey")) {
    pkey = server.arg("pkey");
    pref.putString("pkey", pkey);
  }

  // Save the NTP server address if available
  if (server.hasArg("ntp")) {
    ntp = server.arg("ntp");
    pref.putString("ntp", ntp);
  }

  // Reply with "OK"
  server.send(200, "text/plain", "OK");
}

// Handles AJAX commands for "/cmd/getaccess"
// Sends credentials as a text message
void getAccessData() {
  // Create a message with the credentials, separated by line breaks
  String msg = String(ssid) + "\n" + String(pkey) + "\n" + String(ntp);

  // Reply with the login details
  server.send(200, "text/plain", msg);
}

// Edits AJAX commands for "/cmd/getalarms"
// Sends the alarm times and days of the week for two alarms as a text message
void getAlarms() {
  char buf[45];
  uint8_t h, m, mask, i;

  // Convert the alarm times to hours and minutes and format them
  h = alarm1 / 60;
  m = alarm1 % 60;
  sprintf(buf, "%02i:%02i\n", h, m);

  h = alarm2 / 60;
  m = alarm2 % 60;
  sprintf(&buf[20], "%02i:%02i\n", h, m);

  // Add the days of the week for both alarms
  for (i = 0; i < 7; i++) {
    mask = 1 << i;
    buf[6 + i * 2] = ((alarmday1 & mask) != 0) ? '1' : '0';
    buf[26 + i * 2] = ((alarmday2 & mask) != 0) ? '1' : '0';
    buf[7 + i * 2] = '\n';
    buf[27 + i * 2] = '\n';
  }

  buf[40] = 0;  // Zero termination of the string

  // Reply with the alarm data as a text message
  server.send(200, "text/plain", String(buf));
}

// Calculates the total number of minutes from an hour and minute in the format "HH:MM"
// Returns the minutes as uint16_t
uint16_t stringToMinutes(String val) {
  uint8_t h, m;
  // Extract the hours from the first two characters of the string and convert them to an integer
  h = val.substring(0, 2).toInt();
  // Extract the minutes from the characters after the colon and convert them to an integer
  m = val.substring(3).toInt();
  // Calculate the total number of minutes and return it
  return h * 60 + m;
}

// Processes the AJAX command /cmd/setalarms to set alarm times

  // Save the weekday flags in Settings
  

  // Debug output of weekday flags
  

  // Find the next alarm and display it if Clock mode is active
  

// Processes the AJAX command /cmd/getstation to retrieve data from a specific station
void getStationData() {
  // Checks whether the stationid parameter exists
  if (server.hasArg("stationid")) {
    uint8_t i = server.arg("stationid").toInt();  // Reads the station ID from the request

    // If the station ID is out of range, set it to the last station
    if (i >= STATIONS) i = STATIONS - 1;

    // Create a message with the station's name, URL, and activation status, separated by new lines
    String msg = String(stationlist[i].name) + "\n" + String(stationlist[i].url) + "\n" + String(stationlist[i].enabled) + "\n" + String(i + 1);

    // Send the message as a reply
    server.send(200, "text/plain", msg);
  } else {
    //If the parameter is missing or invalid, send an error message
    server.send(200, "text/plain", "Invalid command");
  }
}

// Processes the AJAX command /cmd/setstation to set data of a specific station
void setStationData() {
  // Expects a valid station ID as an argument
  // Data is also passed as arguments:
  // name = name of the station
  // url = URL of the station
  // enabled = activation status
  // position = new position in the list
  char key[5];

  // Verifies that the station ID exists and is within the valid range
  if ((server.hasArg("stationid")) && (server.arg("stationid").toInt() < STATIONS)) {
    uint8_t i = server.arg("stationid").toInt();

    // Stores the values ​​from the arguments in the station list
    if (server.hasArg("name")) {
      sprintf(key, "n%i", i);
      sender.putString(key, server.arg("name"));
      strlcpy(stationlist[i].name, server.arg("name").c_str(), 32);
    }
    if (server.hasArg("url")) {
      sprintf(key, "u%i", i);
      sender.putString(key, server.arg("url"));
      strlcpy(stationlist[i].url, server.arg("url").c_str(), 150);
    }
    if (server.hasArg("enabled")) {
      sprintf(key, "f%i", i);
      sender.putUChar(key, server.arg("enabled").toInt());
      stationlist[i].enabled = server.arg("enabled").toInt();
    }
    if (server.hasArg("position")) {
      int16_t newpos = server.arg("position").toInt();
      newpos--;
      Serial.printf("Move %i to position %i\n", i + 1, newpos + 1);
      if ((i != newpos) && (newpos >= 0) && (newpos < STATIONS)) {
        reorder(i, newpos);  // Ändert die Position der Station in der Liste
        saveList();          // Speichert die aktualisierte Liste
      }
    }

    // Returns confirmation of successful processing
    server.send(200, "text/plain", "OK");
  } else {
    // Returns an error message if the station ID is invalid
    server.send(200, "text/plain", "Invalid station ID");
  }
}

// Processes the AJAX command /cmd/teststation to test a URL
void testStation() {
  // Expects an argument with the URL to test
  bool ret = true;

  // Checks if the URL argument exists
  if (server.hasArg("url")) {
    // Attempts to start the URL and saves the result
    ret = startUrl(server.arg("url"));
  }

  // Sends a confirmation on success or an error message on failure
  if (ret) {
    // Bei Erfolg: Antwort "OK" senden
    server.send(200, "text/plain", "OK");
  } else {
    // If unsuccessful: Switches back to the current station and sends "ERROR"
    startUrl(String(stationlist[actStation].url));
    server.send(300, "text/plain", "ERROR");
  }
}

// Processes the AJAX command /cmd/endtest to end the test
void endTest() {
  // Switches back to the current station to end the test
  startUrl(String(stationlist[actStation].url));

  //Sends a confirmation that the test has ended
  server.send(200, "text/plain", "OK");
}

// Processes the AJAX command /cmd/restorestations to restore the stations
void restoreStations() {
  restore();  // Stellt die Stationen aus dem Speicher wieder her

  // Sendet eine Bestätigung, dass die Wiederherstellung erfolgreich war
  server.send(200, "text/plain", "OK");
}

// Verarbeitet den AJAX-Befehl /cmd/restart, um das Gerät neu zu starten
void restart() {
  ESP.restart();  // Startet das Gerät neu
}

// Player Tab ######################
// Verarbeitet den AJAX-Befehl /cmd/startPlay, um das Radio zu starten
void startPlay() {
  toggleRadio(false);                    // Schaltet das Radio ein
  server.send(200, "text/plain", "OK");  // Antwortet mit "OK"
}

// Verarbeitet den AJAX-Befehl /cmd/stopPlay, um das Radio zu stoppen
void stopPlay() {
  toggleRadio(true);                     // Schaltet das Radio aus
  server.send(200, "text/plain", "OK");  // Antwortet mit "OK"
}

// Verarbeitet den AJAX-Befehl /cmd/GainSlider, um den Lautstärkewert zu setzen
void GainSlider() {
  // Überprüft, ob der Parameter "GainValue" in der Anfrage vorhanden ist
  if (server.hasArg("GainValue")) {
    // Konvertiert den Wert des Parameters in einen float
    float floatWert = atof(server.arg("GainValue").c_str());
    // Setzt den Lautstärkewert
    setGainValue(floatWert, "WEBSITE");
    // Antwortet mit "OK"
    server.send(200, "text/plain", "OK");

    // Gibt den Wert für Debugging-Zwecke aus
    Serial.print("GainValue: ");
    Serial.println(floatWert);
  } else {
    // Antwortet mit einem Fehlercode, wenn der Parameter fehlt
    server.send(400, "text/plain", "ERROR");
  }
}

// Verarbeitet den AJAX-Befehl /cmd/btnAlarm, um den Alarm ein- oder auszuschalten


// Verarbeitet den AJAX-Befehl /cmd/startSleep, um die Schlummerfunktion zu starten

// Verarbeitet den AJAX-Befehl /cmd/beforeStation, um zur vorherigen "aktiven" Station zu wechseln
void beforeStation() {
  // Zähle die Anzahl der Versuche, um Endlosschleifen zu vermeiden
  int attempts = 0;
  // Schleife, um die vorherige aktive Station zu finden
  do {
    // Verringert die aktuelle Station um eins
    curStation -= 1;
    // Wenn die aktuelle Station kleiner als 0 wird, setze sie auf die letzte Station
    if (curStation < 0) {
      curStation = STATIONS - 1;
    }
    // Increase the attempt counter
    attempts++;
    // When all stations have been run through and no active one has been found
    if (attempts >= STATIONS) {
      // Optional: Reset to the last station (even if it is disabled)
      curStation = STATIONS - 1;
      // Debug output that no active station was found
      Serial.println("Keine aktive Station gefunden."); 
      // Responds with an error message
      server.send(200, "text/plain", "Keine aktive Station verfügbar.");
      return;  // End the function
    }
  } while (!stationlist[curStation].enabled);
  // Changes the station and displays the new station
  changeStation();
  // Debug output of the current station
  Serial.println(curStation); 
  // Answers with "OK"
  server.send(200, "text/plain", "OK");
}

// Processes the AJAX command /cmd/nextStation to move to the next "active" station
void nextStation() {
  // Count the number of attempts to avoid endless loops
  int attempts = 0;
  // Loop to find the next active station
  do {
    // Increases the current station by one
    curStation += 1; 
    // If the current station exceeds the maximum value, set it to the first station
    if (curStation >= STATIONS) {
      curStation = 0;
    }
    // Increase the attempt counter
    attempts++;
    // When all stations have been run through and no active one has been found
    if (attempts >= STATIONS) {
      // Optional: Setze auf die erste Station zurück (auch wenn sie deaktiviert ist)
      curStation = 0;
      // Debug output that no active station was found
      Serial.println("Keine aktive Station gefunden.");
      // Responds with an error message
      server.send(200, "text/plain", "Keine aktive Station verfügbar.");
      return;  // End the function
    }
  } while (!stationlist[curStation].enabled);
  // Changes the station and displays the new station
  changeStation();
  // Debug output of the current station
  Serial.println(curStation);
  // Answers with "OK"
  server.send(200, "text/plain", "OK");
}

// AJAX command /cmd/getCurrentStatus
// This function is called to determine the current status of the system and return it as JSON data.
// The JSON response contains information about:
// - The volume of the radio
// - The alarm status and alarm time
// - The current radio station and title (if the radio is active)
// - Date and time
// - The WiFi signal strength
// - The status of the sleep timer (whether active or not)
void getCurrentStatus() {
  // Variables for time calculation
  uint8_t h, m;
  char txt[50] = "";

  // Creating a JSON document for the response
  StaticJsonDocument<200> jsonDoc;

  // Radio volume
  jsonDoc["gain"] = curGain;

  // Alarm status
  

  // Radio status
  if (radio) {
    jsonDoc["radioStation"] = stationlist[actStation].name;
    jsonDoc["radioTitle"] = title;
  } else {
    jsonDoc["radioStation"] = "";
    jsonDoc["radioTitle"] = "";
  }

  // Current date and time
  char tim[40];
  if (getLocalTime(&ti)) {
    sprintf(tim, "%s %i. %s %i", days[ti.tm_wday], ti.tm_mday, months[ti.tm_mon], ti.tm_year + 1900);
    jsonDoc["Date"] = tim;
    strftime(tim, sizeof(tim), "%H:%M", &ti);
    jsonDoc["Time"] = tim;
  }

  // WiFi signal strength
  jsonDoc["Rssi"] = WiFi.RSSI();

  // Sleep timer status
  jsonDoc["Sleep"] = snoozeWait != 0 ? 1 : 0;

  // Converting the JSON document to a string
  String response;
  serializeJson(jsonDoc, response);

  // Sending the JSON response to the client
  server.send(200, "application/json", response);
}

//################ Info Tab
// AJAX command /cmd/getInfo
// This function is called to return detailed system information as JSON data.
// The JSON response contains:
// - The installed version of the radio
// - Information about the current ESP8266/ESP32 chip, including:
// - Heap size and free heap space
// - Sketch size and free sketch memory
// - chip model
void getInfo() {
  // Creating a JSON document for the response
  StaticJsonDocument<300> jsonDoc;

  // Installed radio version
  jsonDoc["radioversion"] = RADIOVERSION;

  // Creating a sub-object for ESP board information
  JsonObject ESP_INFO = jsonDoc["ESP_INFO"].to<JsonObject>();

  // HEAP information
  JsonObject HEAP = ESP_INFO["HEAP"].to<JsonObject>();
  HEAP["getHeapSize"] = ESP.getHeapSize();  // Total heap size
  HEAP["getFreeHeap"] = ESP.getFreeHeap();  // Free heap space

  // SKETCH Information
  JsonObject SKETCH = ESP_INFO["SKETCH"].to<JsonObject>();
  SKETCH["getSketchSize"] = ESP.getSketchSize();            // Total heap size
  SKETCH["getFreeSketchSpace"] = ESP.getFreeSketchSpace();  // Free heap space

  // CHIP Information
  JsonObject CHIP = ESP_INFO["CHIP"].to<JsonObject>();
  CHIP["getChipModel"] = ESP.getChipModel();  // Model of the chip used

  // Converting the JSON document to a string
  String response;
  serializeJson(jsonDoc, response);

  // Sending the JSON response to the client
  server.send(200, "application/json", response);
}