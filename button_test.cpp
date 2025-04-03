#include "DebouncedInterrupt.h"
#include "mbed.h"

// Define the LED and button
DigitalOut led(PE_8); // LED connected to PE_8
DebouncedInterrupt button1(PA_3);
DebouncedInterrupt button2(PC_1);
DebouncedInterrupt button3(PC_0);

// Function to toggle the LED state
void toggle_led() { led = !led; }

int main() {
  // Attach the debounced interrupt to the button press (falling edge)
  button1.attach(&toggle_led, IRQ_FALL, 20, false); // 20ms debounce time
  button2.attach(&toggle_led, IRQ_FALL, 20, false); // 20ms debounce time
  button3.attach(&toggle_led, IRQ_FALL, 20, false); // 20ms debounce time

  // Enable interrupts
  __enable_irq();

  // Keep the program running indefinitely
  while (true) {
  }
}

/*
pinouts

PA_3
PC_1
PC_0

*/