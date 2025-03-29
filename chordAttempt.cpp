#include "mbed.h"

// Buzzer (PWM) pin
PwmOut speaker(PA_5);

// Sine wave lookup table (128 samples, values from 0 to 255)
const uint8_t sinetable[128] = {
    0, 0, 0, 0, 1, 1, 1, 2, 2, 3, 4, 5, 5, 6, 7, 9,
   10, 11, 12, 14, 15, 17, 18, 20, 21, 23, 25, 27, 29, 31, 33, 35,
   37, 40, 42, 44, 47, 49, 52, 54, 57, 59, 62, 65, 67, 70, 73, 76,
   79, 82, 85, 88, 90, 93, 97, 100, 103, 106, 109, 112, 115, 118, 121, 124,
  128, 131, 134, 137, 140, 143, 146, 149, 152, 155, 158, 162, 165, 167, 170, 173,
  176, 179, 182, 185, 188, 190, 193, 196, 198, 201, 203, 206, 208, 211, 213, 215,
  218, 220, 222, 224, 226, 228, 230, 232, 234, 235, 237, 238, 240, 241, 243, 244,
  245, 246, 248, 249, 250, 250, 251, 252, 253, 253, 254, 254, 254, 255, 255, 255
};

volatile int indexA = 0;  // Index for A4 sine wave
volatile int indexC = 0;  // Index for C5 sine wave
Ticker sineTicker;

// Function to update PWM with a combined sine wave (A4 + C5)
void updatePWM() {
    // Get sine values
    float sineA = sinetable[indexA] / 255.0f;
    float sineC = sinetable[indexC] / 255.0f;
    
    // Mix both waves and normalize (preventing overflow)
    float mixedWave = (sineA + sineC) / 2.0f;  // Average to keep within PWM range
    
    // Output mixed wave
    speaker.write(mixedWave);
    
    // Update indices (128 samples per cycle)
    indexA = (indexA + 1) % 128;  // A4 frequency (440 Hz)
    indexC = (indexC + 150) % 128; // C5 frequency (523.25 Hz) scaled to match table size
}

int main() {
    speaker.period(1.0f / 440.0f);  // Base period for 440 Hz
    sineTicker.attach(&updatePWM, 0.0025f);  // Update PWM every 2.5 ms
    
    while (true) {
        ThisThread::sleep_for(100ms);
    }
}
