#include <Arduino.h>
#include "fonts.h"              // Font headers
#include "tft_color_setting.h"  // Header for TFT color constants
#include "knoepfe.h"            // Graphic data for buttons
#include "symbole.h"            // Graphic data for Symbols
#include "num_64_44.h"          // Graphic data for Favorite Buttons
#include "audio.h"
#include "00_librarys.h"
#include "00_settings.h"    //N Setting
#include "00_pin_settings.h"
#include "00_texte.h"
#include "index.h"

void showConfigPage();
void drawHeaderInfos();

void drawWifiInfo();
void encodeUnicode(const char* src, char* dst);
void updateStation();

void setBrightness(uint16_t value);
void selectStation(uint16_t x);

bool startUrl(String url);

void FavoriteButtons();
boolean radiopage = false;
boolean configpage = false;
const char* PROGMEM months[] = { "Jan.", "Feb.", "Mar", "April", "Maj", "Jun", "Jul", "Aug.", "Sept.", "Okt.", "Nov.", "Dec." };
// Structure for storing alarm configuration

typedef struct {
  uint8_t alarmday_1 = 0B00111110;  // Valid days of the week for Alarm 1 (example 00111110 means Monday to Friday)
  uint8_t h_1;                      // Alarm Hour for Alarm 1
  uint8_t m_1;                      // Alarm Minute for Alarm 1
  uint8_t alarmday_2 = 0B00111110;  // Valid days of the week for Alarm 1 (example 00111110 means Monday to Friday)
  uint8_t h_2;                      // Alarm Hour for Alarm 2
  uint8_t m_2;                      // Alarm Minute for Alarm 2
} AlarmEdit;

AlarmEdit alarmConfig;  // Alarm configuration instance

// Instance of the display driver
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);
// Instance of the Touchscreen driver
XPT2046_Touchscreen touch(TOUCH_CS, TOUCH_IRQ);
// Instance for handling touch events
TouchEvent tevent(touch);

// Prototype of the function to display the slider
void showSlider(uint16_t y, float value, uint16_t vmax, uint16_t bgColor = ILI9341_LIGHTGREY, uint16_t lineColor = ILI9341_BLACK);

// Draws the footer buttons for power, sleep and alarm on the display
// The buttons are displayed in different colors depending on their current status.
// - Power button: Green when the radio is running; Default color if not.
// - Sleep button: Orange when snooze mode is active; Default color if not.
// - Alarm button: default color for active alarm; Red for deactivated alarm.
void DrawFooterButtons_Power_Sleep_Alarm() {
  uint16_t color_temp;  // Variable to store the current color

  // Power Button
  if (radio) {                   // If Radio is running
    color_temp = ILI9341_GREEN;  // Set color for active state
  } else {
    color_temp = COLOR_KNOEPFE;  // Set default color for inactive state
  }
  // Draw the bitmap for the power button
  tft.drawBitmap(0, 176, knopf_sym[0], 64, 64, color_temp, COLOR_KNOEPFE_BG);

  
  // Draw the Bitmap for the Alarm Button
  
}

// Handles interaction with the footer buttons for power, sleep and alarm
// This function checks the x-coordinate of the touch to determine which
// button in the footer area of ​​the display was pressed and performs the corresponding
// action, such as turning the radio on or off, starting snooze mode
// or turning the alarm on or off.
void handleFooterButtons_Power_Sleep_Alarm(int x, int y) {
// Check the x coordinate to identify the button pressed

// Power Button (Position: 0 to 64)
  if (x < 64) {
    toggleRadio(radio);  // Switch the Radio on or off
  }
  
  
}

// Handles touches of favorite buttons on the radio page
// This function checks which of the favorite buttons on the screen was touched
// and sets the current station (`curStation`) to the corresponding favorite.
// The function uses the x and y coordinates of the touch to determine the range of the favorite buttons
// and retrieve the index of active favorites from an array of favorite indices.
void HandleFavoriteButtons(int xpos, int ypos) {
  int loopCount = 0;
  int favoriteIndexes[10];  // Array of 10 elements for the indices of the favorites
  
  // Initialize the array of favorite indices
  for (int i = 0; i < 10; i++) {
    favoriteIndexes[i] = -1;  // A value of -1 indicates that no favorite exists
  }

  // Fill the array with the indices of the active favorites
  for (int i = 0; i < STATIONS && loopCount < 10; i++) {
    if (stationlist[i].enabled) {
      favoriteIndexes[loopCount] = i;
      loopCount++;
    }
  }

  // Check the y-coordinate to determine the range of the favorite buttons
  if (ypos < 88) {  // obere Reihe der Favoriten-Tasten
    // Check the x-coordinate for the specific favorite buttons
    if ((xpos < 64) && (favoriteIndexes[0] > -1)) { curStation = favoriteIndexes[0]; }
    if ((xpos >= 64) && (xpos < 128) && (favoriteIndexes[1] > -1)) { curStation = favoriteIndexes[1]; }
    if ((xpos >= 128) && (xpos < 192) && (favoriteIndexes[2] > -1)) { curStation = favoriteIndexes[2]; }
    if ((xpos >= 192) && (xpos < 256) && (favoriteIndexes[3] > -1)) { curStation = favoriteIndexes[3]; }
    if ((xpos >= 256) && (favoriteIndexes[4] > -1)) { curStation = favoriteIndexes[4]; }
  } else {  // bottom row of favorite buttons
    // Check the x-coordinate for the specific favorite buttons
    if ((xpos < 64) && (favoriteIndexes[5] > -1)) { curStation = favoriteIndexes[5]; }
    if ((xpos >= 64) && (xpos < 128) && (favoriteIndexes[6] > -1)) { curStation = favoriteIndexes[6]; }
    if ((xpos >= 128) && (xpos < 192) && (favoriteIndexes[7] > -1)) { curStation = favoriteIndexes[7]; }
    if ((xpos >= 192) && (xpos < 256) && (favoriteIndexes[8] > -1)) { curStation = favoriteIndexes[8]; }
    if ((xpos >= 256) && (favoriteIndexes[9] > -1)) { curStation = favoriteIndexes[9]; }
  }
}


// Transforms the touch point coordinates according to the current rotation of the TFT display
// This function is used to convert the raw touch coordinates to the screen coordinates,
// based on the rotation of the display. The coordinates are adjusted accordingly,
// so that they are displayed correctly on the screen, regardless of the display rotation.
TS_Point transformTouch(TS_Point p) {
  // Initialize pTransformed with the same values ​​as p
  TS_Point pTransformed = p;

  // Check the current rotation of the TFT display
  switch (tft.getRotation()) {
    case 1:
      // For rotation 1 (90 degrees clockwise):
      // X coordinate is reversed and Y coordinate is also reversed
      pTransformed.x = tft.width() - p.x;
      pTransformed.y = tft.height() - p.y;
      break;
    case 3:
      // For rotation 3 (270 degrees clockwise):
      // No change needed because pTransformed is already p
      break;
    default:
      // Default case for no rotation (0 degrees or 180 degrees):
      // No change needed
      break;
  }

  // Return the transformed coordinates
  return pTransformed;
}



// Callback function for all touch events
// This function processes touch events on the touchscreen and performs appropriate actions
// based on the current page and the position of the touch event.
void onTouchClick(TS_Point p) {
  // First transform the touch coordinates if the display is rotated
  p = transformTouch(p);

  // If the system is not in configuration mode, restart the configuration time
  if (!clockmode) start_conf = millis();

  // Debug output of touched coordinates
  Serial.printf("Touch on %i, %i\n", p.x, p.y);

  if (clockmode) {             // When we are in clock mode
    if (p.y < 210) {           // Check if the click is above 210px
      if (p.x < 40) {          // Left (X < 40)
        beforeStation();       // Calls up the function to switch to the previous station
      } else if (p.x > 280) {  // Right (X > 280)
        nextStation();         // Calls up the function to switch to the next station
      } else {                 // Remaining area above 210px
        clockmode = false;
        configpage = false;
        radiopage = true;
        
        showRadioPage();  // Switch to radio page
      }
    } else {                          // Below 210px
      setGainValue(p.x, "MainPage");  // Adjusts the volume
    }
  } else if (radiopage) {  //################################################ RADIO PAGE
    // When we are in radio mode
    if (p.y > 180) {                                    // If Y coordinate is greater than 180, we are in the button area
      handleFooterButtons_Power_Sleep_Alarm(p.x, p.y);  // Process footer buttons
      if ((p.x > 192) && (p.x < 256)) changeStation();  // Change to the next station
      if (p.x > 256) {                                  // If X coordinate is greater than 256, go to configuration page
        clockmode = false;
        configpage = true;
        radiopage = false;
        
        showConfigPage();
      }
    }
    // Volume control range (0 to 44)
    if ((p.y > 0) && (p.y < 44)) setGainValue(p.x, "RadioPage");
    // Area for favorite buttons (44 to 132)
    if ((p.y > 44) && (p.y < 132)) {
      HandleFavoriteButtons(p.x, p.y);
      changeStation();
      showClock();
    }
    // Area for the station list (132 to 175)
    if ((p.y > 132) && (p.y < 175)) selectStation(p.x);
  } else if (configpage) {  //################################################ CONFIG PAGE
    // When we are in configuration mode
     
    // Volume control range (0 to 44)
    if ((p.y > 0) && (p.y < 44)) setGainValue(p.x, "SettingPage");
    // Brightness control range (44 to 88)
    if ((p.y > 44) && (p.y < 88)) setBrightness(p.x);
    // Snooze timer range (88 to 132)
    
    // Area for the station list (132 to 175)
    if ((p.y > 132) && (p.y < 175)) selectStation(p.x);
  } 
}


// Sets the volume based on the x position where the slider was clicked
// The function calculates the volume value as a percentage and adjusts the slider
// on the appropriate side. The volume value is also saved and updated.
void setGainValue(uint16_t value, const char* sliderPage) {
  char txt[10];
  float v;

  // Check that we are not on the "WEBSITE" page
  if (strcmp(sliderPage, "WEBSITE") != 0) {
    // Calculate the volume value based on the x-position of the slider from 0 to 100%
    int y_start = 11;       // Start line of the slider
    int line_length = 298;  // Length of slider line

    float start_slider = value - y_start;      // Subtract area before line start
    float end_slider = line_length - y_start;  // Subtract area before line start
    v = start_slider / end_slider * 100;       // Calculating the percentage
    if (v > 100) v = 100;                      // Limit the value to a maximum of 100%
    if (v < 0) v = 0;                          // Limit the value to at least 0%
  } else {
    // For the "WEBSITE" page, the value is already given directly
    v = value;
  }

  curGain = v;  //Set the current volume value 
  // Save the current volume value and adjust the slider
  pref.putUShort("gain", curGain);

  // Show the slider depending on the page
  if (strcmp(sliderPage, "MainPage") == 0 || (clockmode && strcmp(sliderPage, "WEBSITE") == 0)) {
    showSlider(218, curGain, 100, COLOR_SLIDER_BG, COLOR_SLIDER);  // Show the slider for the main page or website
  } else {
    // Default: Settings page
    showSlider(27, curGain, 100);         // Show the settings page slider
    sprintf(txt, "%i %%", (int)curGain);  // Format the volume value as text
    // Show the volume value to the right of the slider
    displayMessage(231, 8, 80, 20, txt, ALIGNRIGHT, false, ILI9341_BLACK, ILI9341_LIGHTGREY, 1);
  }

  setGain(curGain);  // Set the volume to the new value
}


//Sets the brightness based on the x position where the slider was clicked
// The function calculates the brightness value as a percentage and adjusts the slider
// accordingly. The brightness value is also saved and displayed.
void setBrightness(uint16_t value) {
  char txt[10];
  // Calculate the brightness based on the x position of the slider from 0 to 100%
  // The slider has a starting value of 15 and the value range is mapped to 0 to 100%
  float v = (value - 15) * 0.345;  // Conversion of the position into percent 

  // Limit the value to the range 0 to 100%
  if (v > 100) v = 100;  // Limit maximum value to 100%
  if (v < 5) v = 0;      // Limit minimum value to 5% (adjustment for possible deviations)

  bright = v;  // Set the current brightness value

  // Save the current brightness value
  pref.putUShort("bright", bright);

  // Display the brightness slider
  showSlider(71, bright, 100);

  //Format the brightness value as text
  sprintf(txt, "%i %%", (int)bright);

  // Display the brightness value to the right of the slider
  displayMessage(231, 52, 80, 20, txt, ALIGNRIGHT, false, ILI9341_BLACK, ILI9341_LIGHTGREY, 1);
}

  //Sets the snooze time based on the x position where the slider was clicked
  // The function calculates the snooze time in minutes and adjusts the slider accordingly.
  // The value is limited to a range of 0 to 60 minutes, saved and displayed.



  // Adjust the display backlight based on percentage
  // If the percentage is 0, the brightness of the display will be reset every 10 seconds,
  // by reading the value from the light dependent resistor (LDR).
  // Otherwise, the brightness is calculated directly from the percentage and scaled to the range 0 to 255.
void setBGLight(uint8_t prct) {
  uint16_t ledb;  // Variable for the brightness of the LED

  // If the percentage is 0, read the ambient brightness from the LDR (Light Dependent Resistor)
  if (prct == 0) {
    // Thanks to René Herzmann for the tip
    /*
    When using the LDR (slider brightness set to “0”), the display began to flicker.
    Therefore, the brightness of the display is only reset every 10 seconds.
    */
    static unsigned long prevMillis = -20000;   // Timestamp of the last brightness measurement
    if (millis() - prevMillis < 10000) return;  // Wait 10 seconds before measuring brightness again
    prevMillis = millis();                      // Update the timestamp
    ledb = analogRead(LDR) * 255 / 4096;        // Read the LDR value and scale it to 0 to 255
  } else {
    // Calculate brightness based on percentage
    ledb = 255 * prct / 100;  // Scale the percentage to a range of 0 to 255
  }

  // Limit the brightness value to the range of 3 to 255
  if (ledb > 255) ledb = 255;  // Limit maximum value to 255
  if (ledb < 3) ledb = 3;      // Limit minimum value to 3 to avoid fading

  // If LED_ON is set to 0, invert the brightness
  if (LED_ON == 0) ledb = 255 - ledb;

  // Debug output of brightness
  Serial.printf("percent = %i LED = %i\n", prct, ledb);

  // Set the brightness of the backlight
  analogWrite(TFT_LED, ledb);
}

  // Selects a station from the list of stations based on the x position of the touch event
  // The left button (x < 50) scrolls down the list of stations,
  // while the right button (x > 270) scrolls up the list.
  // The selection is set to the next activated station and the display is updated.
void selectStation(uint16_t x) {
  // Check whether the x position is in the area of ​​the left button
  if (x < 50) {  // Left button to scroll down the stations
    // Go down the list until an activated station is found
    do {
      if (curStation == 0) {
        // If we are at the beginning of the list, go to the end of the list
        curStation = STATIONS - 1;
      } else {
        // Otherwise, go to the previous station
        curStation--;
      }
    } while (!stationlist[curStation].enabled);  // Repeat until an activated station is found
  }

  // Check whether the x position is in the area of ​​the right button
  if (x > 270) {  // Right button to scroll up the stations
    // Move up through the list until an activated station is found
    do {
      if (curStation == (STATIONS - 1)) {
        // When we get to the end of the list, go to the top of the list
        curStation = 0;
      } else {
        // Otherwise, go to the next station
        curStation++;
      }
    } while (!stationlist[curStation].enabled);  // Repeat until an activated station is found
  }

  // Show the selected station
  updateStation();
}

//Turns the radio on or off based on the boolean parameter 'off'.
// If 'off' is true, the radio is stopped, the snooze time is reset and the radio status is set to 'off'.
// If 'off' is false, turns on the radio, starts the current station URL (or switches to the first station if starting fails),
// and the volume is set to the current value.
// The mode is then set to clock and the clock display is displayed.

void toggleRadio(boolean off) {
  if (off) {
    // Wenn 'off' wahr ist, stoppe das Radio
    stopPlaying();   // Stop the current stream
    snoozeWait = 0;  // Reset the snooze time
    radio = false;   // Set radio status to 'off'
    //setGain(0);  // (Optional) Set the volume to 0 (commented out)
  } else {
    // If 'off' is false, turn on the radio
    if (connected) {  // Check if there is a connection
      radio = true;   // Set radio status to 'on'
      if (!startUrl(String(stationlist[actStation].url))) {
        // If starting the URL fails, go to the first station
        actStation = 0;                                 // Set the current station to 0
        startUrl(String(stationlist[actStation].url));  // Try launching the URL again
      }
      setGain(curGain);  // Set the volume to the current value
    }
  }
  // Switch to clock display
  clockmode = true;  // Set the mode to Clock
  showClock();       // Show the clock display
}

//Turns the alarm on or off based on the current value of 'alarmday'.
// If 'alarmday' is greater than 7 (meaning the alarm is currently disabled),
// the next available alarm is found and the alarm status is set to 'on'.
// If 'alarmday' is 7 or less (which means the alarm is enabled),
// the alarm is disabled by setting 'alarmday' to 8.
// The alarm status is updated accordingly in the settings.
// The mode is then set to clock and the clock display is displayed.


// Starts the snooze function. Sets the snooze time to the current value of 'snoozeTime',
// stops the radio from playing by calling the 'toggleRadio(false)' function,
// and then switches to clock mode by setting 'clockmode' to true and displaying the clock display.

//Sets the selected station as the active station, saves this value and tries to start the URL of the current station.
// If starting the URL fails, it switches to station 0. The mode is then set to clock and the clock display is displayed.
void changeStation() {
  actStation = curStation;  // Sets the current station (curStation) as the active station (actStation)

  // Saves the new value of the active station
  pref.putUShort("station", curStation);

  // Attempts to start the URL of the current station
  if (!startUrl(String(stationlist[actStation].url))) {
    // If startup fails, switch to station 0
    actStation = 0;
    startUrl(String(stationlist[actStation].url));  // Starts the URL of station 0
  }

  // Switches to the clock screen
  clockmode = true;
  showClock();  // Displays the clock display
}

// Called in the main loop to query and process touch events from the touchscreen.
void touch_loop() {
  tevent.pollTouchScreen();  // Queries the touchscreen for events
}

// Prepares the display for use by initializing the TFT display and touchscreen module,
// setting the backlight and performing the display configuration.
void setup_display() {
  pinMode(TFT_LED, OUTPUT);  // Sets the pin for the backlight of the TFT display to output
  pinMode(LDR, INPUT);       // Sets the pin for the light sensor (LDR) to input

  lastldr = analogRead(LDR);  // Reads the current value of the light sensor (LDR) and stores it

  setBGLight(100);  // Sets the backlight to 100% brightness
  
  tft.begin();                    // Initializes the TFT display
  tft.fillScreen(ILI9341_BLACK);  // Fills the entire screen with the color black
  tft.setRotation(3);             // Sets the display rotation to 3 (may vary depending on the display)
  tft.setFont(&AT_Bold12pt7b);    // Sets the font for the text to a custom font

  touch.begin();                                    // Initializes the touchscreen module
  tevent.setResolution(tft.width(), tft.height());  // Sets the resolution for the touch event module
  tevent.setDrawMode(false);                        // Sets the drawing mode for the touch event module to off (drawing is disabled)

  // Registers the callback function for touch events
  tevent.registerOnTouchClick(onTouchClick);
}

// Draws a text in a rectangle at position x,y with size w,h
// The text can be left-aligned, centered or right-aligned
// The text size can be set to standard or large
// Font color and background color can be selected as well as the number of lines
void textInBox(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const char* text, uint8_t align = ALIGNLEFT, boolean big = false, uint16_t fc = ILI9341_WHITE, uint16_t bg = ILI9341_BLACK, uint8_t lines = 1) {
  char tmp[256];               // Temporary array to store the encoded text
  char msg[50];                // Temporary array to store the current row
  char* token;                 // Pointer for tokenizing the text
  int16_t xt, yt, h0, x0, sp;  // Variables for text calculations
  uint16_t wt, ht, len;        // Variables for text width and height
  uint8_t l;                   // Number of remaining rows

  // Fills the rectangle with the background color to delete old content
  tft.fillRect(x, y, w, h, bg);

  // Sets the font based on the `big` variable
  if (big) tft.setFont(FNT12);  // Large font
  else tft.setFont(FNT9);       // Standard font

  tft.setTextColor(fc);  // Sets the text color

  // Encoding for German umlauts
  encodeUnicode(text, tmp);

  // Determines the width and height of a single letter (e.g. "n") to calculate the spacing
  tft.getTextBounds("n", 0, 100, &xt, &yt, &wt, &ht);
  sp = wt;  // Spacing between characters

  // Determines the width and height of the entire text
  tft.getTextBounds(tmp, 0, 100, &xt, &yt, &wt, &ht);

  //Calculates the y offset based on the text height
  h0 = 100 - yt;
  x0 = x;
  y = y + h0 + 1;  // Sets the starting point for drawing the text

  if (wt < w) {                                    // Enough space for the entire text on one line
    if (align == ALIGNCENTER) x0 += (w - wt) / 2;  // Centers the text
    if (align == ALIGNRIGHT) x0 += (w - wt);       // Right-aligned
    tft.setCursor(x0, y);                          // Sets the cursor to the calculated position
    tft.print(tmp);                                // Draws the text
  } else {                                         // Not enough space for all text on one line
    // Text is adjusted by word wrapping if only one line is allowed
    l = lines;
    token = strtok(tmp, " ");  // Breaks the text into individual words
    msg[0] = 0;                // Clears the message array
    len = 0;                   // Initializes the length of the current line
    while ((token != NULL) && (l > 0)) {
      tft.getTextBounds(token, 0, 100, &xt, &yt, &wt, &ht);
      if ((len + wt + sp) < w) {  // Checks if the word fits in the current line
        if (len > 0) {
          strcat(msg, " ");  // Adds a space if necessary
          len += sp;         // Adds space between words
        }
        len = len + wt;
        strcat(msg, token);  // Adds the word to the current line
      } else {
        x0 = x;
        if (align == ALIGNCENTER) x0 += (w - len) / 2;  // Centers the line
        if (align == ALIGNRIGHT) x0 += (w - len);       // Right-aligned
        tft.setCursor(x0, y);                           // Sets the cursor for the new line
        tft.print(msg);                                 // Draws the current line
        len = wt;                                       // Sets the length to the width of the current word
        strcpy(msg, token);                             // Starts a new line with the current word
        y = y + ht + 1;                                 // Moves the y-position for the next line
        l--;                                            // Reduces the remaining number of rows
      }
      token = strtok(NULL, " ");  // Get the next word
    }
    if ((len > 0) && (l > 0)) {  // Draws the last line if there is space
      x0 = x;
      if (align == ALIGNCENTER) x0 += (w - len) / 2;  // Centers the line
      if (align == ALIGNRIGHT) x0 += (w - len);       // Right-aligned
      y = y + h0 + 1;                                 // Moves the y-position for the last line
      tft.setCursor(x0, y);                           // Sets the cursor for the last line
      tft.print(msg);                                 // Draws the last line
    }
  }
}

// Updates the time on the display. To avoid flickering, only the changed parts are redrawn.
// If the `redraw` parameter is set to true, the entire content is redrawn.
void updateTime(boolean redraw) {
  char tim[40];  // Temporary array for time and date representation

  // These variables are static to retain their value between calls.
  static char lastdate[40] = "";
  static char lasttime[10] = "";

  // Gets the local time and stores it in the structure `ti`.
  if (getLocalTime(&ti)) {
    // Formatting the date as "Weekday Day. Month Year"
    sprintf(tim, "%s %i. %s %i", days[ti.tm_wday], ti.tm_mday, months[ti.tm_mon], ti.tm_year + 1900);

    // Checks if the display should be redrawn or if the date has changed.
    // If so, the date line is redrawn.
    if (redraw || (strcmp(tim, lastdate) != 0)) {
      strcpy(lastdate, tim);                                                       // Stores the new date in `lastdate`
      textInBox(0, 64, 320, 13, tim, ALIGNCENTER, true, COLOR_DATE, COLOR_BG, 1);  // DATE!! Records the date, mod 0, 90, 320, 25,
    }

    uint8_t z;                                 // Variable for storing the digit value
    strftime(tim, sizeof(tim), "%H:%M", &ti);  // Formats the current time as "HH:MM"
    Serial.printf("Zeit = %s\n", tim);         // Prints the current time to the serial console

    // Loops through the time string to check each digit.
    // If `redraw` is true or a digit has changed, that digit is redrawn.
    for (uint8_t i = 0; i < 5; i++) {
    // Checks if the current position in the time string is a hyphen (":").
    // Sets `z` to 10 for the hyphen or to the numeric digit.
      z = (i == 2) ? 10 : tim[i] - '0';
      if ((z < 11) && (redraw || (tim[i] != lasttime[i]))) {
        // Draws the digits on the display. The digit images are provided by `ziffern[z]`.
        tft.drawBitmap(45 + i * 45, 12, ziffern[z], 40, 52, COLOR_TIME, COLOR_BG);      //Key line for Time display
      }
    }

    drawHeaderInfos();      // Draws symbols and text in the header
    strcpy(lasttime, tim);  // Stores the new time in `lasttime`
  }
}

// Records the information in the header of the display, including wifi information, sleep icon and next alarm time.
void drawHeaderInfos() {
  drawWifiInfo();    // Records the current Wifi information in the header
  
}

// Records the Wifi information in the header of the display
void drawWifiInfo() {
  int rssi = WiFi.RSSI();                            // Get the RSSI value for the signal strength
  char rssiChar[5];                                  // Char array to store the RSSI value as text
  snprintf(rssiChar, sizeof(rssiChar), "%d", rssi);  // Convert RSSI value to rssiChar

  uint16_t color_wifi;  // Color variable for the wifi symbol
  //Determine the color based on the signal strength (RSSI value)
  if (rssi <= -70) {  // Weak reception (e.g. red)
    color_wifi = ILI9341_RED;
  } else if (rssi <= -50) {  // Medium reception (e.g. yellow)
    color_wifi = ILI9341_YELLOW;
  } else {  // Strong reception (e.g. green)
    color_wifi = ILI9341_GREEN;
  }

  // Draw the Wifi symbol at position (303, 0) with the specified color
  tft.drawBitmap(303, 0, symbole[0], 17, 17, color_wifi, COLOR_BG);

  // Draw the RSSI value as text in the box (position (273, 0) with size (30, 17))
  textInBox(273, 0, 30, 17, rssiChar, ALIGNCENTER, false, COLOR_WIFI_RSSI, COLOR_BG);

  // Draw the local IP address in the box (position (97, 0) with size (159, 17))
  String localIPString = WiFi.localIP().toString();  // Hole die lokale IP-Adresse als String
  textInBox(97, 0, 159, 17, localIPString.c_str(), ALIGNCENTER, false, COLOR_IP, COLOR_BG);
}

// Draws the sleep icon in the header of the display


// Clears the entire screen and resets it to the background color
void displayClear() {
  // Fills the entire screen with the background color
  tft.fillScreen(COLOR_BG);
}

// Displays date and time at the designated position
void displayDateTime() {
  Serial.println("Show Time");  // Debug output for the start of the time display
  updateTime(false);            // Updates the time without completely redrawing
}

// Displays the progress of a software update as a bar and percentage
void showProgress(uint32_t prc) {
  char tmp[20];

  // Formatting the progress percentage as text
  sprintf(tmp, "Progress %i%%", prc);

  // Shows the progress text at position (5, 50) on the display
  textInBox(5, 50, 310, 30, tmp, ALIGNLEFT, true, ILI9341_GREEN, COLOR_BG, 1);

  // Draws the progress bar based on the progress
  if (prc == 0) {
    // Only draws the outline of the progress bar when the progress is 0%
    tft.drawRect(5, 80, 310, 20, ILI9341_BLUE);
  } else {
    // Fills the progress bar according to progress
    tft.fillRect(5, 80, 310 * prc / 100, 20, ILI9341_BLUE);
  }
}

// Encodes text from Unicode to the display code format for the specified font
// The source and destination buffers must exist
void encodeUnicode(const char* src, char* dst) {
  uint8_t i = 0;             // Index for the source array
  uint8_t i1 = 0;            // Index for the target array
  char c;                    // Current sign
  uint16_t m = strlen(src);  // Length of source string

  do {
    c = src[i];

    // Checks for UTF-8 characters for German umlauts
    if (c == 195) {
      i++;
      switch (src[i]) {
        case 164: c = 130; break;  // ä -> 130
        case 182: c = 131; break;  // ö -> 131
        case 188: c = 132; break;  // ü -> 132
        case 159: c = 133; break;  // ß -> 133
        case 132: c = 127; break;  // Ä -> 127
        case 150: c = 128; break;  // Ö -> 128
        case 156: c = 129; break;  // Ü -> 129
        default: c = 255;          // Unknown character, not encoded
      }
    } else if (c == 194) {  // Checks for UTF-8 characters for the degree symbol
      i++;
      if (src[i] == 176) c = 134;  // Degree symbol -> 134
      else c = 255;                // Unknown character
    }

    if (c < 135) dst[i1] = c;  // Only characters in the range < 135 are written to the destination buffer
    i++;
    i1++;
  } while (i < m);  // Loop to the end of the source string
  dst[i1] = 0;  // Null termination of the target string
}

  // Displays a slider. y defines the upper line, value is the position of the rectangle
void showSlider(uint16_t y, float value, uint16_t vmax, uint16_t bgColor, uint16_t lineColor) {
  // Fills the background of the slider with the background color
  tft.fillRect(11, y, 298, 14, bgColor);

  // Draws the horizontal line of the slider
  tft.drawLine(15, y + 6, 305, y + 6, lineColor);  // Top of the line
  tft.drawLine(15, y + 7, 305, y + 7, lineColor);  // Bottom edge of the line

  // Calculates the position of the slider based on the value
  uint16_t pos = value * 290 / vmax;

  // Draws the slider rectangle at the calculated position
  tft.fillRect(12 + pos, y, 8, 14, lineColor);
}

// Displays the volume display area on the configuration screen
void showGain() {
  char txt[20];

  // Fill the background of the area with the color COLOR_SETTING_BG
  tft.fillRect(0, 0, 320, 44, COLOR_SETTING_BG);

  // Draw a border around the area
  tft.drawRect(0, 0, 320, 44, COLOR_SETTING_BORDER);

  // Format volume value in percent and save in txt
  sprintf(txt, "%i %%", curGain);

  // Display the volume value right-aligned in an area
  textInBox(231, 8, 80, 20, txt, ALIGNRIGHT, false, ILI9341_BLACK, COLOR_SETTING_BG, 1);

  // Set cursor position and font for the label
  tft.setCursor(5, 23);
  tft.setFont(FNT9);
  tft.setTextColor(ILI9341_BLACK);

  // Encode and display text for the Volume label
  encodeUnicode("Lautstärke", txt);
  tft.print(txt);

  // Show the slider to represent the current volume
  showSlider(27, curGain, 100);
}

// Displays the volume display area on the main screen
void showGainMain() {
  // Fills an area on the screen (box) with the background color COLOR_SLIDER_BG
  tft.fillRect(0, 210, 320, 30, COLOR_SLIDER_BG);

  // Draws a border around the previously filled area with the color COLOR_SLIDER_BORDER
  tft.drawRect(0, 210, 320, 30, COLOR_SLIDER_BORDER);

  // Displays a slider that represents the current volume (curGain).
  // The slider starts at y-position 218, has a maximum value of 100
  // and uses the colors COLOR_SLIDER_BG and COLOR_SLIDER
  showSlider(218, curGain, 100, COLOR_SLIDER_BG, COLOR_SLIDER);
}

// Displays the brightness display area on the configuration screen
void showBrigthness() {
  char txt[20];

  // Fills a rectangular area on the screen with the background color COLOR_SETTING_BG
  tft.fillRect(0, 44, 320, 44, COLOR_SETTING_BG);

  // Draws a border around the previously filled area with the color COLOR_SETTING_BORDER
  tft.drawRect(0, 44, 320, 44, COLOR_SETTING_BORDER);

  // Formats the brightness as a percentage and saves it to txt
  sprintf(txt, "%i %%", bright);

  // Displays the formatted text in a box, right-aligned
  textInBox(231, 52, 80, 20, txt, ALIGNRIGHT, false, ILI9341_BLACK, COLOR_SETTING_BG, 1);

  // Set cursor for the Text
  tft.setCursor(5, 67);

  // Selects the font and text color
  tft.setFont(FNT9);
  tft.setTextColor(ILI9341_BLACK);

  // Encodes the text "Brightness" into the target array txt to represent umlauts
  encodeUnicode("Helligkeit", txt);

  // Prints the text "Brightness" on the screen
  tft.print(txt);

  // Displays a slider to represent the current brightness
  showSlider(71, bright, 100);
}

// Function to display snooze time on the configuration screen

// Function to display the selected station in the station area
void updateStation() {
  char txt[40];  // Station name buffer

  // Encode the station name (e.g. for umlauts) and save it in the buffer
  encodeUnicode(stationlist[curStation].name, txt);

  // Show coded station name in station area
  textInBox(40, 145, 240, 20, txt, ALIGNCENTER, false, ILI9341_BLACK, COLOR_SETTING_BG, 1);
}

// Function to display station list in settings screen
void showStationList() {
  // Draw area for the station list
  tft.fillRect(0, 132, 320, 44, COLOR_SETTING_BG);      // Set background color
  tft.drawRect(0, 132, 320, 44, COLOR_SETTING_BORDER);  // Rahmen zeichnen

  // Show up and down scroll icons
  tft.drawBitmap(0, 132, symbole[4], 50, 44, COLOR_SETTING_UP_DOWN);    // Up arrow
  tft.drawBitmap(270, 132, symbole[5], 50, 44, COLOR_SETTING_UP_DOWN);  // Down arrow

  // Set current station as selected station
  curStation = actStation;

  // Show selected station
  updateStation();
}

// Function to display the full configuration screen
void showConfigPage() {
  setBGLight(100);           // Set backlight brightness to 100%
  tft.fillScreen(COLOR_BG);  // Fill the screen background with the defined color

  showGain();         // Show volume range
  showBrigthness();   // Show brightness range
  
  showStationList();  // Show station list display area

  

  // Draw radio button
  tft.drawBitmap(192, 176, knopf_sym[4], 64, 64, COLOR_KNOEPFE, COLOR_KNOEPFE_BG);

  // Draw a button to switch to the alarm screen (clock display).
  

  start_conf = millis();  // Record the start time of the configuration page
}

// Function to display the full radio screen
void showRadioPage() {
  setBGLight(100);                   // Set backlight brightness to 100%
  tft.fillScreen(COLOR_KNOEPFE_BG);  // Fill the screen background with the defined button color

  showGain();         // Show volume range
  FavoriteButtons();  // Show favorite buttons
  showStationList();  // Show area to display the station list

  DrawFooterButtons_Power_Sleep_Alarm();  // Draw buttons for power, sleep and alarm in the footer

  // Draw radio button
  tft.drawBitmap(192, 176, knopf_sym[4], 64, 64, COLOR_KNOEPFE, COLOR_KNOEPFE_BG);

  // Draw button to go to settings
  tft.drawBitmap(256, 176, knopf_sym[6], 64, 64, COLOR_KNOEPFE, COLOR_KNOEPFE_BG);

  start_conf = millis();  // Record the start time of the radio configuration page
}

// Function to display the full alarm screen



// Function to display alarm days and times

// Toggles a day for the alarm


// Increments or decrements the alarm hours


// Increments or decrements the alarm minutes in 5 minute increments

// Saves the changed alarm time and displays 'OK'


// Displays the name of the active station on the TFT display
void showStation() {
  textInBox(4, 86, 312, 12, stationlist[actStation].name, ALIGNCENTER, true, COLOR_STATION_NAME, COLOR_STATION_BOX_BG);//modified 5,64,310,30
}

// Displays the current metadata on the TFT display
void showTitle() {
  displayMessage(4, 108, 312, 96, title, ALIGNCENTER, true, COLOR_STATION_TITLE, COLOR_STATION_BOX_BG, 3); //modified 5,94,310,120
}

// Displays the radio section on the clock screen
void showRadio() {
  if (radio) {
    tft.fillRect(1, 84, 318, 126, COLOR_STATION_BOX_BG);      // Fill the background of the radio area, modified 3,117,314,93
    tft.drawRect(1, 84, 318, 126, COLOR_STATION_BOX_BORDER);  // Draw the frame of the radio area,  modified dtto
    showStation();                                            // Show active station name
    showTitle();                                              // Show title of current station
  } else {
    tft.fillRect(1, 84, 318, 126, COLOR_BG);  // Set background of radio area to background color when radio is off
  }
}



// Can be used to display three integers on the bottom line of the display
void showDebugInfo(int16_t v1, int16_t v2, int16_t v3) {
  char txt[100] = "";                                                                      // Text buffer for display
  if (clockmode) {                                                                         // Show only when clock mode is active
    sprintf(txt, "Info: v1 = %i, v2 = %i, v3 = %i", v1, v2, v3);                           // Create formatted debug information
    textInBox(0, 220, 320, 20, txt, ALIGNCENTER, false, ILI9341_WHITE, ILI9341_BLACK, 1);  // Show debug information
  }
}

// Redraw the clock screen
void showClock() {
  start_conf = 0;            // Reset the configuration start time
  setBGLight(bright);        // Set the backlight to the current brightness
  tft.fillScreen(COLOR_BG);  // Fill the entire screen with the background color value
  updateTime(true);          // Update and display the time
  if (radio) showRadio();    // Show the radio section when radio mode is active
  // showNextAlarm has been moved to updateTime (only needed every minute)
  showGainMain();  // Show the current volume in the main area
}


// A covering function for text in the box that can be used by other sub-functions
void displayMessage(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const char* text, uint8_t align, boolean big, uint16_t fc, uint16_t bg, uint8_t lines) {
  textInBox(x, y, w, h, text, align, big, fc, bg, lines);  // Call the function to draw the text in the box
}

// Create favorite buttons on the radio list between y44 and y132 and switch directly to them
void FavoriteButtons() {
  int y = 44;           // Start Y position for the buttons
  bool active = false;  // Status variable whether the button is active
  int loopCount = 0;    // Counter for the loop

  for (int i = 0; i < STATIONS && loopCount < 10; i++) {
    if (stationlist[i].enabled) {  // Check whether the station button is activated
      active = (actStation == i);  // Activate button for the current station
      tft.drawBitmap((loopCount % 5) * 64, y, num_64_44[loopCount], 64, 44,
                     active ? COLOR_FAV_BUTTONS_AKTIV : COLOR_FAV_BUTTONS, COLOR_FAV_BUTTONS_BG);
      loopCount++;               // Increment loop counters
      if (loopCount % 5 == 0) {  // Move to the next line every 5 buttons
        y = 88;                  // Set Y position for next row
      }
    }
  }
}
