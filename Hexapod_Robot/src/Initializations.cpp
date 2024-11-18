#include <Arduino.h>
#include <header.h>

// =============================================================================
//        Định nghĩa các đối tượng Servo, Sử dụng thư viện Servo.h

Adafruit_PWMServoDriver servoDriver_0 = Adafruit_PWMServoDriver();
Adafruit_PWMServoDriver servoDriver_1 = Adafruit_PWMServoDriver(0x41);

Servo coxa1;
Servo femur1;
Servo tibia1;

Servo coxa2;
Servo femur2;
Servo tibia2;

Servo coxa3;
Servo femur3;
Servo tibia3;

Servo coxa4;
Servo femur4;
Servo tibia4;

Servo coxa5;
Servo femur5;
Servo tibia5;

Servo coxa6;
Servo femur6;
Servo tibia6;

// Định nghĩa các pin
const int coxa1Pin = 0; 
const int femur1Pin = 1;
const int tibia1Pin = 2;

const int coxa2Pin = 4;
const int femur2Pin = 5;
const int tibia2Pin = 6;

const int coxa3Pin = 8;
const int femur3Pin = 9;
const int tibia3Pin = 10;

const int coxa4Pin = 0;
const int femur4Pin = 1;
const int tibia4Pin = 2;

const int coxa5Pin = 4;
const int femur5Pin = 5;
const int tibia5Pin = 6;

const int coxa6Pin = 8;
const int femur6Pin = 9;
const int tibia6Pin = 10;

// Định nghĩa các Vector3
const Vector3 offsets1 = {90,75,-18};
const Vector3 offsets2 = {93,75,-15};
const Vector3 offsets3 = {93,75,-18}; 
const Vector3 offsets4 = {87,80,-26};
const Vector3 offsets5 = {85,89,-16};
const Vector3 offsets6 = {93,85,-24};
const Vector3 offsets[6] = {offsets1, offsets2, offsets3, offsets4, offsets5, offsets6};

const float a1 = 41;  //Coxa Length
const float a2 = 116; //Femur Length
const float a3 = 183; //Tibia Length   
float legLength = a1 + a2 + a3;

Vector3 currentPoints[6];
Vector3 cycleStartPoints[6];

Vector3 currentRot(180, 0, 180);
Vector3 targetRot(180, 0, 180);

float strideMultiplier[6] = {1, 1, 1, -1, -1, -1};
float rotationMultiplier[6] = {-1, 0, 1, -1, 0 , 1};

Vector3 ControlPoints[10];
Vector3 RotateControlPoints[10];
Vector3 AttackControlPoints[10];

// Định nghĩa hàm attachServos
int microsecondsToPWM(int microseconds) {
    return map(microseconds, 500, 2500, 123, 615); // 123 và 615 tương ứng với 500-2500us
}


void attachServos(){

  //====Init======
  servoDriver_0.begin();
  servoDriver_0.setPWMFreq(50); // Tần số 50Hz cho servo

  servoDriver_1.begin();
  servoDriver_1.setPWMFreq(50); // Tần số 50Hz cho servo
  //==============
  coxa1.attach(coxa1Pin,500,2500);
  femur1.attach(femur1Pin,500,2500);
  tibia1.attach(tibia1Pin,500,2500); 

  coxa2.attach(coxa2Pin,500,2500);
  femur2.attach(femur2Pin,500,2500);
  tibia2.attach(tibia2Pin,500,2500);  

  coxa3.attach(coxa3Pin,500,2500);
  femur3.attach(femur3Pin,500,2500);
  tibia3.attach(tibia3Pin,500,2500);

  coxa4.attach(coxa4Pin,500,2500);
  femur4.attach(femur4Pin,500,2500);
  tibia4.attach(tibia4Pin,500,2500);

  coxa5.attach(coxa5Pin,500,2500);
  femur5.attach(femur5Pin,500,2500);
  tibia5.attach(tibia5Pin,500,2500);

  coxa6.attach(coxa6Pin,500,2500);
  femur6.attach(femur6Pin,500,2500);
  tibia6.attach(tibia6Pin,500,2500);  
}
