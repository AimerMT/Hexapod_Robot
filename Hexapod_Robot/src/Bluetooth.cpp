#include <Arduino.h>
#include "header.h"

#define rxBluetooth PA10  // Chân RX của STM32
#define txBluetooth PA9   // Chân TX của STM32

SoftwareSerial bluetoothSerial(rxBluetooth, txBluetooth);

void Bluetooth_init()
{
    bluetoothSerial.begin(9600); // Kết nối HC-05
}
