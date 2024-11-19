#ifndef SERVOS_H
#define SERVOS_H

#include <Arduino.h>
#include <Adafruit_PWMServoDriver.h>
#include <Wire.h>
// Khai báo các đối tượng Servo

extern Adafruit_PWMServoDriver servoDriver_0;
extern Adafruit_PWMServoDriver servoDriver_1;

// Khai báo các pin
extern const int coxa1Pin;
extern const int femur1Pin;
extern const int tibia1Pin;

extern const int coxa2Pin;
extern const int femur2Pin;
extern const int tibia2Pin;

extern const int coxa3Pin;
extern const int femur3Pin;
extern const int tibia3Pin;

extern const int coxa4Pin;
extern const int femur4Pin;
extern const int tibia4Pin;

extern const int coxa5Pin;
extern const int femur5Pin;
extern const int tibia5Pin;

extern const int coxa6Pin;
extern const int femur6Pin;
extern const int tibia6Pin;

// Khai báo các Vector3
extern const Vector3 offsets1;
extern const Vector3 offsets2;
extern const Vector3 offsets3;
extern const Vector3 offsets4;
extern const Vector3 offsets5;
extern const Vector3 offsets6;
extern const Vector3 offsets[6];

extern const float a1;  //Coxa Length
extern const float a2;  //Femur Length
extern const float a3;  //Tibia Length   
extern float legLength;

extern Vector3 currentPoints[6];
extern Vector3 cycleStartPoints[6];

extern Vector3 currentRot;
extern Vector3 targetRot;

extern float strideMultiplier[6];
extern float rotationMultiplier[6];

extern Vector3 ControlPoints[10];
extern Vector3 RotateControlPoints[10];
extern Vector3 AttackControlPoints[10];

// Khai báo hàm
int microsecondsToPWM(int microseconds);
void attachServos();

#endif // SERVOS_H
