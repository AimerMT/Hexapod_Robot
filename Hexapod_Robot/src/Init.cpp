#include <Arduino.h>
#include <header.h>

// =============================================================================
//        Định nghĩa các đối tượng Servo, Sử dụng thư viện Servo.h

Adafruit_PWMServoDriver servoDriver_0 = Adafruit_PWMServoDriver(0x41);
Adafruit_PWMServoDriver servoDriver_1 = Adafruit_PWMServoDriver(0x40);


// Định nghĩa các pin
//                                    |       
//                      210      4|       |3     012
//                             |            |  
//                      210   5|            |2   012
//                             |            |
//                      210   6|            |1   012
//                                | | | | |


const int coxa1Pin = 0;  //Khop 1 
const int femur1Pin = 1; //Khop 2
const int tibia1Pin = 2; //Khop 3

const int coxa2Pin = 4;
const int femur2Pin = 5;
const int tibia2Pin = 6;

const int coxa3Pin = 12;
const int femur3Pin = 13;
const int tibia3Pin = 14;

const int coxa4Pin = 0;
const int femur4Pin = 1;
const int tibia4Pin = 2;

const int coxa5Pin = 8;
const int femur5Pin = 9;
const int tibia5Pin = 10;

const int coxa6Pin = 12;
const int femur6Pin = 13;
const int tibia6Pin = 14;

// Định nghĩa các Vector3
const Vector3 offsets1 = {80,70,-5}; // 90 75 -18
const Vector3 offsets2 = {90,90,-10}; 
const Vector3 offsets3 = {90,85,-10}; 
const Vector3 offsets4 = {80,90,170};
const Vector3 offsets5 = {80,90,170};
const Vector3 offsets6 = {80,85,170};
const Vector3 offsets[6] = {offsets1, offsets2, offsets3, offsets4, offsets5, offsets6};



Vector3 currentPoints[6];
Vector3 cycleStartPoints[6];

Vector3 currentRot(180, 0, 180);
Vector3 targetRot(180, 0, 180);

float strideMultiplier[6] = {1, -1, -1, -1, 1, 1}; //1, 1, 1, -1, -1, -1
//float strideMultiplier_1[6] = {-1, -1, -1, -1, -1, -1};
float rotationMultiplier[6] = {-1, 1, 1, -1, 1 , 1}; //-1, 0, 1, -1, 0 , 1

Vector3 ControlPoints[10];
Vector3 RotateControlPoints[10];
Vector3 AttackControlPoints[10];

int microsecondsToPWM(int microseconds) {
    microseconds = constrain(microseconds, 500, 2500);
    return map(microseconds, 500, 2500, 102, 512); // 
}
bool servosAttached = false;
void attachServos(){

  //====Init======
  servoDriver_0.begin();
  servoDriver_0.setOscillatorFrequency(27000000);
  servoDriver_0.setPWMFreq(50); // Tần số 50Hz cho servo

  servoDriver_1.begin();
  servoDriver_1.setOscillatorFrequency(27000000);
  servoDriver_1.setPWMFreq(50); // Tần số 50Hz cho servo
  //==============

  servosAttached = true;
}

void detachServos() {
    // Tắt tất cả các servo được quản lý bởi servoDriver_0
    for (int channel = 0; channel < 16; channel++) {
        servoDriver_0.setPWM(channel, 0, 4096); // Giá trị 4096 dùng để tắt kênh PWM
    }

    // Tắt tất cả các servo được quản lý bởi servoDriver_1
    for (int channel = 0; channel < 16; channel++) {
        servoDriver_1.setPWM(channel, 0, 4096); // Giá trị 4096 dùng để tắt kênh PWM
    }
    servosAttached = false;
}
