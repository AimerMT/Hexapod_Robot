#pragma once
#include <SPI.h>
#include <RF24.h>
#include <Arduino.h>
#define Current_Sensor_Pin 0

// RF24 radio setup
extern RF24 radio; // CE, CSN pin
extern uint8_t address[][6]; // Node addresses
extern bool radioNumber;

// Timing
extern unsigned long rc_last_received_time;
extern unsigned long rc_timeout;

// Data structures for RC and Hexapod
struct RC_Data_Package {
  byte joy1_X;
  byte joy1_Y;
  byte joy1_Button;
  byte joy2_X;
  byte joy2_Y;
  byte joy2_Button;
  byte slider1;
  byte slider2;
  byte pushButton1;
  byte pushButton2;
};

struct Hexapod_Data_Package {
  float current_sensor_value;
};

// Instances of the data structures
extern RC_Data_Package rc_data;
extern RC_Data_Package rc_data_previous;
extern Hexapod_Data_Package hex_data;

void RC_Setup();
bool RC_GetDataPackage();
bool GetData();
void RC_DisplayData();
void RC_ResetData();
void SendData();
void initializeHexPayload();


