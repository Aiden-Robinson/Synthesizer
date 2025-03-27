#include "mbed.h"

// Buzzer (PWM) pin
PwmOut speaker(PA_5); // PWM output to speaker

// Sine wave lookup table (128 samples, values from 0 to 255)
const uint8_t sinetable[128] = {
    0,   0,   0,   0,   1,   1,   1,   2,   2,   3,   4,   5,   5,   6,   7,
    9,   10,  11,  12,  14,  15,  17,  18,  20,  21,  23,  25,  27,  29,  31,
    33,  35,  37,  40,  42,  44,  47,  49,  52,  54,  57,  59,  62,  65,  67,
    70,  73,  76,  79,  82,  85,  88,  90,  93,  97,  100, 103, 106, 109, 112,
    115, 118, 121, 124, 128, 131, 134, 137, 140, 143, 146, 149, 152, 155, 158,
    162, 165, 167, 170, 173, 176, 179, 182, 185, 188, 190, 193, 196, 198, 201,
    203, 206, 208, 211, 213, 215, 218, 220, 222, 224, 226, 228, 230, 232, 234,
    235, 237, 238, 240, 241, 243, 244, 245, 246, 248, 249, 250, 250, 251, 252,
    253, 253, 254, 254, 254, 255, 255, 255};

volatile int index = 0; // Index to iterate through sine table
Ticker sineTicker;      // Ticker to update the PWM output

// Function to update the PWM output with sine wave values
void updatePWM() {
  speaker.write(sinetable[index] /
                255.0f);     // Normalize sine value to 0.0 to 1.0
  index = (index + 1) % 128; // Loop through the sine table
}

int main() {
  speaker.period(1.0f / 440.0f); // Start with 440 Hz (A4) frequency
  speaker.write(0.5f);           // Set duty cycle to 50% for constant volume

  // Update PWM sine wave every 10 ms
  sineTicker.attach(&updatePWM, 0.0025f); // Update sine wave every 10 ms

  while (true) {
    // Main loop does nothing, everything is handled by the ticker
    ThisThread::sleep_for(100ms);
  }
}