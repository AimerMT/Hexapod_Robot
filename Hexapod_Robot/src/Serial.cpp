// #include <Arduino.h>
// #include <header.h>

// int Data[11];

// SoftwareSerial MySerial(rx, tx);

// size_t indexOfCurDataByte = 0;
// bool readData = false;

// void MySerial_init()
// {
//     MySerial.begin(9600); // Default communication rate of the Bluetooth module
// }

// // read one Byte from Bluetooth module
// void MySerial_read()
// {
//     // while data is available from the Bluetooth module
//     while (MySerial.available())
//     {
//         if (!readData)
//         {
//             if (MySerial.read() == 0) // 0 defines te begining of a new array of data being send. Without it we would get out of sync
//             {
//                 readData = true;
//                 indexOfCurDataByte = 0;
//             }
//         }
//         else // we are in sync so we read the next byte
//         {
//             int data = MySerial.read();

//             if (indexOfCurDataByte >= DATA_LENGTH || data == 0) // we have reached the end of the expected receive data
//             {
//                 readData = false;
//                 indexOfCurDataByte = 0;

//                 MySerial_clear();
//             }

//             Data[indexOfCurDataByte] = data;

//             indexOfCurDataByte++;
//         }
//     }
// }

// // clear old Bluetooth  data
// void MySerial_clear()
// {
//     while (MySerial.available())
//     {
//         MySerial.read();
//     }
// }

// // maps receive data to directionInput etc.
// void MySerial_map()
// {
//     // // transfer received data to input Values for walking gait
//     // int stickZero = 100;
//     // int minValue = 20;

//     // maxSpeed = Data[5];

//     // rotationInput = float(Data[1] - stickZero) / float(stickZero - minValue) * maxSpeed * -1;

//     // directionInput.x = float(Data[4] - stickZero) / float(stickZero - minValue) * maxSpeed;
//     // directionInput.y = float(Data[3] - stickZero) / float(stickZero - minValue) * maxSpeed;

//     // directionInput = directionInput.inverse();

//     // groundClearance = Data[6];
//     // stepRadius = Data[7];
// }
