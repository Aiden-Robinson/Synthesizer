#include "mbed.h"

PwmOut speaker(PA_5);             // PWM output to speaker
InterruptIn user_button(BUTTON1); // Onboard button for toggling arpeggiator

// Frequencies for the A major chord (A, C#, E)
float frequencies[] = {440.0f, 554.37f, 659.25f}; // A, C#, E
int numNotes = sizeof(frequencies) /
               sizeof(frequencies[0]); // Number of notes in the chord
int currentNote = 0;                   // To track the current note being played
Ticker noteTicker;                     // Ticker to update the note output
volatile bool arpeggiatorActive = true; // Flag to control arpeggiator state

// Function to play the current note
void playNote() {
  if (arpeggiatorActive) { // Check if arpeggiator is active
    // Set the PWM frequency to the current note
    speaker.period(1.0f / frequencies[currentNote]);
    speaker.write(0.5f); // Set duty cycle to 50% for constant volume

    // Move to the next note in the chord
    currentNote = (currentNote + 1) % numNotes; // Cycle through A, C#, E
  } else {
    speaker.write(0.0f); // No sound when arpeggiator is inactive
  }
}

// Interrupt handler to toggle arpeggiator state
void toggleArpeggiator() {
  arpeggiatorActive = !arpeggiatorActive; // Toggle the state
}

int main() {
  // Attach the interrupt handler to the button press
  user_button.rise(
      &toggleArpeggiator); // Call toggleArpeggiator on button press

  // Update note every 200 ms
  noteTicker.attach(&playNote, 0.2f); // Update note every 200 ms

  while (true) {
    // ... existing code ...
  }
}