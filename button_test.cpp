#include "mbed.h"
#include "DebouncedInterrupt.h"

// Define the LED and button
DigitalOut led(PE_8); // LED connected to PC_13
DebouncedInterrupt button(PE_14); // External push button on PA_0

// Function to toggle the LED state
void toggle_led() {
    led = !led;
}

int main() {
    // Attach the debounced interrupt to the button press (falling edge)
    button.attach(&toggle_led, IRQ_FALL, 20, false); // 20ms debounce time
    
    // Enable interrupts
    __enable_irq();
    
    // Keep the program running indefinitely
    while (true) {
    }
}
