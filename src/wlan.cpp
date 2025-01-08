// Initializes the WiFi module with the specified SSID and passkey
#include <Arduino.h>
#include <WiFi.h>  // Library for WiFi functions
#include "audio.h"
#include "00_settings.h"  //N Setting
#define TXT_DISCONNECT "Disconnecting WiFi..."
#define TXT_SOFT_AP_DISCONNECT "Disconnecting Soft AP..."
#define TXT_SET_WLAN_MODE "Setting WLAN mode..."
#define TXT_CONNECTING_TO "Connecting to SSID: "
#define TXT_CONNECTING_ESTABLISH "..."
#define TXT_IP_ADDRESS "IP Address: "
#define TXT_NO_CONNECTION "No connection established."

boolean initWiFi(String ssid, String pkey) {
  boolean connected = false;  // Variable to track connection status

  // First, disconnect all existing connections
  Serial.print(TXT_DISCONNECT);          // Debug output that the connection is being disconnected
  WiFi.disconnect();                     // Disconnects the current WiFi connection
  Serial.print(TXT_SOFT_AP_DISCONNECT);  // Debug output that Soft AP mode is disconnected
  WiFi.softAPdisconnect(true);           // Disconnects Soft AP mode if active

  //Sets the WiFi mode to Station (Client) and configures the hostname
  Serial.print(TXT_SET_WLAN_MODE);  // Debug output that the WLAN mode is set
  WiFi.setHostname(NETWORK_NAME);   // Sets the hostname for the device
  WiFi.mode(WIFI_STA);              // Sets WiFi mode to Station (Client)

  Serial.print(TXT_CONNECTING_TO);           // Debug output that a connection to an SSID is being established
  Serial.print(ssid);                        // Prints the SSID on the console
  Serial.println(TXT_CONNECTING_ESTABLISH);  // Debug output that the connection is being established
  Serial.println(pkey);                      // Outputs the passkey to the console (Note: it is not safe to output the passkey to the console)

  // If an SSID was specified, attempt to connect
  if (ssid != "") {
    WiFi.begin(ssid.c_str(), pkey.c_str());  // Begins the attempt to connect to the specified SSID with the passkey
    uint8_t cnt = 0;                         // Counter for the number of attempts
    // Try to connect up to 20 times
    while ((WiFi.status() != WL_CONNECTED) && (cnt < 20)) {
      delay(500);         // Wait 500ms 
      Serial.print(".");  // Displays progress on the console
      cnt++;              // Increase the counter
    }
    Serial.println();  // New line for clean console output
    // If the connection is successfully established
    if (WiFi.status() == WL_CONNECTED) {
      Serial.print(TXT_IP_ADDRESS);    // Debug output of the IP address
      Serial.println(WiFi.localIP());  // Prints the local IP address on the console
      connected = true;                // Sets the connection status to true
    }
  }

  // If a connection could not be established
  if (!connected) {
    Serial.println(TXT_NO_CONNECTION);  // Debug output that a connection could not be established
    WiFi.mode(WIFI_AP);                 // Sets WiFi mode to Access Point (AP)
    WiFi.softAP(AP_NAME, "", 1);        // Starts the access point with the specified name and without a password
  }

  return connected;  // Returns the connection status (true if connected, false otherwise)
}