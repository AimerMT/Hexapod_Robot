#include <Arduino.h>
#include <header.h>

// RF24 radio(PB0, PA4); // CE, CSN pins
// const byte address[5] = { '0', '0', '0', '1', '1' }; // Hợp lệ
// bool radioNumber = 1;

// // Timing
// unsigned long rc_last_received_time = 0;
// unsigned long rc_timeout = 1000;

// // Data structures for RC and Hexapod
 PacketData receivedData;
 RC_Data_Package rc_data;
 RC_Data_Package rc_data_previous;
// //Hexapod_Data_Package hex_data;

 void RC_Setup(){
   RC_ResetData();
//   radio.setPALevel(RF24_PA_LOW);
//   radio.setPayloadSize(sizeof(rc_data));
//   radio.setChannel(124);
//   radio.openReadingPipe(1, address); // Đúng địa chỉ
//   radio.enableAckPayload();
//   radio.startListening();
//   //initializeHexPayload();
//   //radio.writeAckPayload(1, &hex_data, sizeof(hex_data));
 }

// // void initializeHexPayload(){
// //   hex_data.current_sensor_value = 0;
// // }

// //bool GetData(){  
//   // This device is a RX node

//   //=========Always True=================
//   // uint8_t pipe;
//   // if (radio.available(&pipe)) {
//   //   uint8_t bytes = radio.getPayloadSize(); // get the size of the payload
//   //   radio.read(&rc_data, bytes);            // fetch payload from FIFO 
//   //   //RC_DisplayData();
//   //   hex_data.current_sensor_value = mapFloat(analogRead(Current_Sensor_Pin),0,1024,0,50);
//   //   radio.writeAckPayload(1, &hex_data, sizeof(hex_data)); // load the payload for the next time
//   //   rc_last_received_time = millis();
//   // }
//   // if(millis() - rc_last_received_time >  rc_timeout) return false;
//   //======================================
  
//   //return true;
// //}

// //void RC_DisplayData(){
//   // Serial.print("Joy1 X: ");
//   // Serial.print(rc_data.joy1_X);

//   // Serial.print(" | Joy1 Y: ");
//   // Serial.print(rc_data.joy1_Y);

//   // Serial.print(" | Joy1 Button: ");
//   // Serial.print(rc_data.joy1_Button);

//   // Serial.print(" | Joy2 X: ");
//   // Serial.print(rc_data.joy2_X);

//   // Serial.print(" | Joy2 Y: ");
//   // Serial.print(rc_data.joy2_Y);

//   // Serial.print(" | Joy2 Button: ");
//   // Serial.print(rc_data.joy2_Button);

//   // Serial.print(" | Pot 1: ");
//   // Serial.print(rc_data.slider1);

//   // Serial.print(" | Pot 2: ");
//   // Serial.print(rc_data.slider2);

//   // Serial.print(" | Button 1: ");
//   // Serial.print(rc_data.pushButton1);

//   // Serial.print(" | Button 2: ");
//   // Serial.println(rc_data.pushButton2);
// //}

void RC_ResetData(){
  rc_data.joy1_X = 127;
  rc_data.joy1_Y = 127;
  // rc_data.joy1_Button = 0;

  rc_data.joy2_X = 127;
  rc_data.joy2_Y = 127;
  receivedData.lxAxisValue = 127;
  receivedData.lyAxisValue = 127;
  receivedData.rxAxisValue = 127;
  receivedData.ryAxisValue = 127;
  // rc_data.joy2_Button = 0;

  // rc_data.slider1 = 0; //40
  // rc_data.slider2 = 25;

  // rc_data.pushButton1 = 0;
  // rc_data.pushButton2 = 0;
}

