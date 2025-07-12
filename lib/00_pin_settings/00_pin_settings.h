// Pin-Definitionen for different Components of the Project

// Pin for the Light sensor (LDR)
#define LDR 35 //36

// Pin for the Buzzer on the original Az-Delivery TouchMod
//#define BEEPER 21

// Audio-Pins
#define LRCLK 25  // Left-Right Clock for the Audio Output
#define BCLK 26   // Bit Clock for the Audio Output
#define DOUT 33   // Data output for the Audio Output

// Pins for the Touchscreen
#define TOUCH_CS 33   //14 Chip Select for the Touchscreen
#define TOUCH_IRQ 36  //27 Interrupt-Output for the Touchscreen

// Pins for TFT-Display
#define TFT_CS 15    //5 Chip Select for the TFT-Display
#define TFT_DC 2    //4 Data/Command Pin for the TFT-Display
#define TFT_RST -1  //22 Reset-Pin for the TFT-Display
#define TFT_LED 21  //15 Backlight fo the TFT-Displays
#define LED_ON 0    // Logic level for the backlight (0 = Ein)