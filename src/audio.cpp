#include "audio.h"
#include <AudioGeneratorMP3.h>
#include <AudioFileSourceICYStream.h>
#include <AudioFileSourceBuffer.h>
#include <AudioOutputI2S.h>
#include "00_pin_settings.h"  
#include "00_settings.h"      //N
#include "index.h"          //N Index page

// Buffer size for stream buffering
const int preallocateBufferSize = 82 * 1024;  // Size of the preallocated buffer in bytes
const int preallocateCodecSize = 29192;       // Maximum memory required for the MP3 codec in bytes

// Pointer to pre-allocated memory
void *preallocateBuffer = NULL;  // Pointer to the pre-allocated buffer for the data
void *preallocateCodec = NULL;   // Pointer to the pre-allocated buffer for the Codec

// Audio component instances
AudioGenerator *decoder = NULL;         // MP3-Decoder
AudioFileSourceICYStream *file = NULL;  // Input module for ICY streams from the web
AudioFileSourceBuffer *buff = NULL;     // Buffer for the input stream
AudioOutputI2S *out;                    // Output module for generating I2S signals

// Function for all the required setup
void setup_audio() {
  // Memmory allocation for Decoder and Stream
  preallocateBuffer = malloc(preallocateBufferSize);  // Request memory for the stream file buffer
  preallocateCodec = malloc(preallocateCodecSize);    // Request memory for the decoder buffer
  delay(1000);                                        // Wait 1 second to ensure memory has been allocated

  // Verify that memory was allocated successfully
  if (!preallocateBuffer || !preallocateCodec) {
    Serial.printf_P(PSTR("FATAL ERROR: Unable to preallocate %d bytes for app\n"), preallocateBufferSize + preallocateCodecSize);
    while (1) {
      yield();  // Infinite loop to fix the error
    }
  }

  // Create I2S output to use with the decoder
  out = new AudioOutputI2S();         // New AudioOutputI2S-Objekt set
  out->SetPinout(BCLK, LRCLK, DOUT);  // Set pin configuration for I2S output
}

// This function checks whether the decoder is running
void audio_loop() {
  // Check whether the decoder is running
  if (decoder->isRunning()) {
    // If the decoder is running, check whether the loop is still running
    if (!decoder->loop()) {
      // When the loop stops running, stop the decoder
      decoder->stop();
    }
  } else {
    // The decoder has stopped, indicating an error
    Serial.printf("MP3 done\n");
    // To avoid the same problem occurring again after a reboot,
    // the station is set to the first entry in the list.
    // The first entry should be a stable, working URL.
    // Do not use this first entry for experiments.
    pref.putUShort("station", 0);

    // Wait 10 seconds before restarting the ESP
    delay(10000);

    // New start the ESP
    ESP.restart();
  }
}

// Callback function called when metadata is found in the input stream
void MDCallback(void *cbData, const char *type, bool isUnicode, const char *string) {
  // Make sure the pointers are valid
  if (type == nullptr || string == nullptr) {
    Serial.println("Error: Null pointer encountered in MDCallback.");
    return;
  }

  // Note: The type and string can be in PROGMEM, hence copy to RAM for printf
  char s1[32], s2[64];
  strncpy_P(s1, type, sizeof(s1) - 1);
  s1[sizeof(s1) - 1] = 0;
  strncpy_P(s2, string, sizeof(s2) - 1);
  s2[sizeof(s2) - 1] = 0;

  // Processing metadata type "Title"
  if (strstr_P(s1, PSTR("Title"))) {
    strncpy(title, s2, sizeof(title));
    title[sizeof(title) - 1] = 0;
    Serial.printf("Title: %s\n", title);
    // Show the message on the display
    newTitle = true;
  } else {
    // Show only the type and string for generic metadata
    Serial.printf("METADATA '%s' = '%s'\n", s1, s2);
  }

  // Serial.flush(); // Optional: For flushing the serial output if necessary
}

// Stops playing the input stream, frees the memory, and deletes the instances
void stopPlaying() {
  if (decoder) {
    decoder->stop();  // Stops playing
    delete decoder;   // Frees the decoder and its memory
    decoder = NULL;   // Sets the pointer to NULL to indicate that memory has been freed
  }

  if (buff) {
    buff->close();  // Closes the buffer
    delete buff;    // Frees the buffer and its memory
    buff = NULL;    // Sets the pointer to NULL
  }

  if (file) {     // And finally for the stream
    file->close();  // Closes the stream
    delete file;    // Frees the Stream and its memory
    file = NULL;    // Sets the pointer to NULL
  }   
}

// Starts playing a stream from the specified station
bool startUrl(String url) {
  bool ret = true;
  stopPlaying();  // First close existing streams

  // Open the input file for the selected URL
  Serial.printf("Active station %s\n", url.c_str());
  file = new AudioFileSourceICYStream();
  ret = file->open(url.c_str());

  if (ret) {
    // Register callback for metadata
    file->RegisterMetadataCB(MDCallback, NULL);

    // Create a new buffer that uses the pre-allocated memory
    buff = new AudioFileSourceBuffer(file, preallocateBuffer, preallocateBufferSize);
    Serial.printf_P(PSTR("sourcebuffer created - Free mem=%d\n"), ESP.getFreeHeap());

    // Create and start a new decoder
    decoder = (AudioGenerator *)new AudioGeneratorMP3(preallocateCodec, preallocateCodecSize);
    Serial.printf_P(PSTR("created decoder\n"));
    Serial.printf_P("Decoder start...\n");
    delay(1000);
    decoder->begin(buff, out);
  } else {
    delete file;  // If unsuccessful, delete the file
    file = NULL;
  }

  return ret;
}

// Changes the volume to the current gain value
void setGain(float gain) {
  float v = gain / 100 * 0.5;                // Calculates the volume as a percentage of 0.5
  out->SetGain(v);                           // Sets the volume
  Serial.printf("New volume = %4.2f\n", v);  // Outputs the new volume value
}
