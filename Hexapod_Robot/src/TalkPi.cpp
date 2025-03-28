#include <Arduino.h>
#include "header.h"

#define rxPi PA3  // Chân RX của STM32
#define txPi PA2   // Chân TX của STM32

SoftwareSerial TalkPi(rxBluetooth, txBluetooth);

void Serial_init()
{
    TalkPi.begin(9600); 
}
