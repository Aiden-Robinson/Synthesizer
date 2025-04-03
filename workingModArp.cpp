#include "DebouncedInterrupt.h"
#include "mbed.h"

// Buzzer (PWM) pin
PwmOut speaker(PA_5);

// Potentiometers
AnalogIn pot(PA_4);    // Controls arpeggiator speed
AnalogIn modPot(PA_6); // Controls modulation speed

// Push buttons for toggling notes
DebouncedInterrupt button1(PF_7);
DebouncedInterrupt button2(PF_9);
DebouncedInterrupt button3(PC_0);
DebouncedInterrupt button4(PC_1);
DebouncedInterrupt button5(PA_3);

// LEDs corresponding to notes
DigitalOut led1(PD_9);
DigitalOut led2(PB_13);
DigitalOut led3(PB_11);
DigitalOut led4(PE_15);
DigitalOut led5(PE_13);

// Sine wave lookup table (128 samples)
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

volatile int index = 0;
Ticker sineTicker;

// Frequencies for the notes in the chord
float frequencies[] = {293.66f, 370.00f, 440.0f, 554.37f, 659.25f};
bool noteEnabled[5] = {true, true, true, true,
                       true}; // Track which notes are enabled

// Function to update PWM output
void updatePWM() {
  speaker.write(sinetable[index] / 255.0f);
  index = (index + 1) % 128;
}

// Function to update modulation speed
void updateModulationSpeed() {
  float modValue = modPot.read(); // Read potentiometer value (0.0 to 1.0)
  float newSpeed =
      0.005f + modValue * 10 * (0.1f - 0.005f); // Scale to range 0.005 - 0.1
  sineTicker.attach(&updatePWM, newSpeed); // Adjust the sine wave update rate
}

// Toggle functions for each note
void toggleNote1() { noteEnabled[0] = !noteEnabled[0]; }
void toggleNote2() { noteEnabled[1] = !noteEnabled[1]; }
void toggleNote3() { noteEnabled[2] = !noteEnabled[2]; }
void toggleNote4() { noteEnabled[3] = !noteEnabled[3]; }
void toggleNote5() { noteEnabled[4] = !noteEnabled[4]; }

int main() {
  // Attach buttons to toggle functions
  button1.attach(&toggleNote1, IRQ_FALL, 20, false);
  button2.attach(&toggleNote2, IRQ_FALL, 20, false);
  button3.attach(&toggleNote3, IRQ_FALL, 20, false);
  button4.attach(&toggleNote4, IRQ_FALL, 20, false);
  button5.attach(&toggleNote5, IRQ_FALL, 20, false);

  // Enable interrupts
  __enable_irq();

  // Start PWM sine wave generation
  updateModulationSpeed(); // Initialize modulation speed
  // sineTicker.attach(&updatePWM, 0.1f);

  // LED array corresponding to notes
  DigitalOut *leds[] = {&led5, &led4, &led3, &led2, &led1};

  int currentNote = 0;
  while (true) {
    float potValue = pot.read();
    int delayTime = 50 + (int)(potValue * 800);

    // Adjust modulation speed dynamically
    updateModulationSpeed();

    // Find next enabled note
    do {
      currentNote = (currentNote + 1) % 5;
    } while (!noteEnabled[currentNote]);

    // Set speaker frequency
    speaker.period(1.0f / frequencies[currentNote]);

    // Update LEDs
    for (int i = 0; i < 5; i++) {
      *leds[i] = (i == currentNote) ? 1 : 0;
    }

    // Wait before switching notes
    ThisThread::sleep_for(delayTime);
  }
}
