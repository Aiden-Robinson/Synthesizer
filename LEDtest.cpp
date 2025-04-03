
#include "mbed.h"

DigitalOut led1(PD_9);
DigitalOut led2(PB_13);
DigitalOut led3(PB_11);
DigitalOut led4(PE_15);
DigitalOut led5(PE_13);

int main() {

  while (true) {
    led1 = 1;
    led2 = 1;
    led3 = 1;
    led4 = 1;
    led5 = 1;
  }
}
