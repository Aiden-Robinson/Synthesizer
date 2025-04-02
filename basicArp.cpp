#include "mbed.h"

// Buzzer (PWM) pin
PwmOut speaker(PA_5); // PWM output to speaker

// Potentiometer pin (PA_0)
AnalogIn pot(PA_0); // Reading the potentiometer value

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

// Frequencies for the A major chord (A, C#, E)
float frequencies[] = {440.0f, 554.37f, 659.25f}; // A, C#, E
int numNotes = sizeof(frequencies) / sizeof(frequencies[0]); // Number of notes in the chord
int currentNote = 0; // To track the current note being played

// Function to update the PWM output with sine wave values
void updatePWM() {
  speaker.write(sinetable[index] / 255.0f); // Normalize sine value to 0.0 to 1.0
  index = (index + 1) % 128; // Loop through the sine table
}

int main() {
  // Set the PWM frequency to the first note in the chord (A)
  speaker.period(1.0f / frequencies[currentNote]);
  speaker.write(0.5f); // Set duty cycle to 50% for constant volume

  // Update PWM sine wave every 10 ms
  sineTicker.attach(&updatePWM, 0.025f); // Update sine wave every 10 ms

  while (true) {
    // Read the potentiometer value (range 0.0 to 1.0)
    float potValue = pot.read();

    // Use the potentiometer value to control the speed of the arpeggio
    // Map potValue to a delay range (e.g., 100ms to 1000ms)
    int delayTime = 50 + (int)(potValue * 800); // Map to 100ms to 1000ms

    // Loop through the notes of the A major chord to create the arpeggio
    speaker.period(1.0f / frequencies[currentNote]); // Update the PWM frequency

    // Move to the next note in the chord
    currentNote = (currentNote + 1) % numNotes; // Cycle through A, C#, E

    // Wait before switching to the next note (based on potentiometer input)
    ThisThread::sleep_for(delayTime); // Delay between notes based on potentiometer value
  }
}
