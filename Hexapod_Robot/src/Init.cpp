#include <Arduino.h>
#include <header.h>

// =============================================================================
//        Định nghĩa các đối tượng Servo, Sử dụng thư viện Servo.h

Adafruit_PWMServoDriver servoDriver_0 = Adafruit_PWMServoDriver(0x40);
Adafruit_PWMServoDriver servoDriver_1 = Adafruit_PWMServoDriver(0x41);


// Định nghĩa các pin
//                                    |       
//                      210      1|       |2     012
//                             |            |  
//                      210   6|            |3   012
//                             |            |
//                      210   5|            |4   012
//                                | | | | |


const int coxa1Pin = 0;  //Khop 1 
const int femur1Pin = 1; //Khop 2
const int tibia1Pin = 2; //Khop 3

const int coxa2Pin = 4;
const int femur2Pin = 5;
const int tibia2Pin = 6;

const int coxa3Pin = 8;
const int femur3Pin = 9;
const int tibia3Pin = 10;

const int coxa4Pin = 12;
const int femur4Pin = 13;
const int tibia4Pin = 14;

const int coxa5Pin = 4;
const int femur5Pin = 5;
const int tibia5Pin = 6;

const int coxa6Pin = 8;
const int femur6Pin = 9;
const int tibia6Pin = 10;

// Định nghĩa các Vector3
const Vector3 offsets1 = {60,0,-90}; //90 75 -18
const Vector3 offsets2 = {80,0,90}; 
const Vector3 offsets3 = {93,75,-18}; 
const Vector3 offsets4 = {87,80,-26};
const Vector3 offsets5 = {85,89,-16};
const Vector3 offsets6 = {93,85,-24};
const Vector3 offsets[6] = {offsets1, offsets2, offsets3, offsets4, offsets5, offsets6};



Vector3 currentPoints[6];
Vector3 cycleStartPoints[6];

Vector3 currentRot(180, 0, 180);
Vector3 targetRot(180, 0, 180);

float strideMultiplier[6] = {1, 1, 1, -1, -1, -1};
float rotationMultiplier[6] = {-1, 0, 1, -1, 0 , 1};

Vector3 ControlPoints[10];
Vector3 RotateControlPoints[10];
Vector3 AttackControlPoints[10];

int microsecondsToPWM(int microseconds) {
    return map(microseconds, 500, 2500, 102, 512); // 
}

void attachServos(){

  //====Init======
  servoDriver_0.begin();
  servoDriver_0.setPWMFreq(50); // Tần số 50Hz cho servo

  servoDriver_1.begin();
  servoDriver_1.setPWMFreq(50); // Tần số 50Hz cho servo
  //==============
}
