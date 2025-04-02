#include "mbed.h"

// Define the event queue and event flags
EventFlags eventFlags;

// Define the tasks
void defaultTask() {
  while (true) {
    // Wait for the event flag from the Ticker
    eventFlags.wait_any(0x1); // Wait for the Ticker event
    // Toggle the onboard green LED (assuming it's connected to a pin)
    // ... code to toggle green LED ...
  }
}

void myTask02() {
  while (true) {
    // Wait for the event flag from the push button interrupt
    eventFlags.wait_any(0x2); // Wait for the button event
    // Toggle the onboard red LED (assuming it's connected to a pin)
    // ... code to toggle red LED ...
  }
}

// Ticker interrupt function
void ticker1Callback() {
  eventFlags.set(0x1); // Set the event flag for the default task
}

// External interrupt callback for the push button
void buttonCallback() {
  eventFlags.set(0x2); // Set the event flag for myTask02
}

int main() {
  // Set up the Ticker
  Ticker ticker1;
  ticker1.attach(&ticker1Callback, 1.0f); // Trigger every 1 second

  // Set up the external interrupt for the push button on PA_7
  InterruptIn button(PA_7);     // Using PA_7 as the push button pin
  button.rise(&buttonCallback); // Trigger on rising edge

  // Create the default task and myTask02
  Thread defaultThread;
  defaultThread.start(defaultTask);

  Thread myTask02Thread;
  myTask02Thread.start(myTask02);
}
