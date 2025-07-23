// Pin-Definitionen for different Components of the Project

// Pin for the Light sensor (LDR)
#define LDR 34

// Pin for the Buzzer on the original Az-Delivery TouchMod
#define BEEPER 26

// Audio-Pins
#define LRCLK 25  // Left-Right Clock for the Audio Output
#define BCLK 26   // Bit Clock for the Audio Output
#define DOUT 33   // Data output for the Audio Output

// Pins for the Touchscreen
#define TOUCH_CS 33   // Chip Select for the Touchscreen
#define TOUCH_IRQ 36  // Interrupt-Output for the Touchscreen

// Pins for TFT-Display
#define TFT_CS 15    // Chip Select for the TFT-Display
#define TFT_DC 2    // Data/Command Pin for the TFT-Display
#define TFT_RST -1  // Reset-Pin for the TFT-Display
#define TFT_LED 21  // Backlight fo the TFT-Displays
#define TFT_MISO 12  // MISO Pin for the TFT-Display
#define TFT_MOSI 13  // MOSI Pin for the TFT-Display
#define TFT_SCLK 14   // SCLK Pin for the TFT-Display
#define LED_ON 0    // Logic level for the backlight (0 = Ein)
