#pragma once
#include <Arduino.h>
#include "Helpers.h"
#include "RC.h"
#include "vectors.h"
#include "Init.h"
#include <SoftwareSerial.h>
#include <Wire.h>


#define FUNCTIONS_H_

#ifdef FUNCTIONS_H_

#define Current_Sensor_Pin 0


enum State {
  Initialize,
  Stand,
  Car,
  Crab,
  Calibrate,
  SlamAttack
};

enum LegState {
  Propelling,
  Lifting,
  Standing,
  Reset
};

enum Gait {
  Tri,
  Wave,
  Ripple,
  Bi,
  Quad,
  Hop  
};
// ================================================================
// ===                          Vecto                           ===
// ================================================================



// ================================================================
// ===                            main                          ===
// ================================================================
extern int totalGaits;
extern Gait gaits[6];  // Danh sách các gait

    // Các biến liên quan đến các điểm và tiến độ chu kỳ
extern float points;
extern int cycleProgress[6];
extern LegState legStates[6];
extern int standProgress;

// Các trạng thái hiện tại
extern State currentState;
extern Gait currentGait;
extern Gait previousGait;
extern int currentGaitID;

// Điều chỉnh khoảng cách khi đứng
extern float standingDistanceAdjustment;

// Khoảng cách từ mặt đất
extern float distanceFromGroundBase;
extern float distanceFromGround;
extern float previousDistanceFromGround;

// Chiều cao nâng và hạ
extern float liftHeight;
extern float landHeight;
extern float strideOvershoot;
extern float distanceFromCenter;

// Các biến liên quan đến điều khiển bằng tay
extern float crabTargetForwardAmount;
extern float crabForwardAmount;

extern Vector2 joy1TargetVector;
extern float joy1TargetMagnitude;
extern Vector2 joy1CurrentVector;
extern float joy1CurrentMagnitude;

extern Vector2 joy2TargetVector;
extern float joy2TargetMagnitude;
extern Vector2 joy2CurrentVector;
extern float joy2CurrentMagnitude;

// Thời gian liên quan đến đầu vào và chu kỳ
extern unsigned long timeSinceLastInput;
extern float landingBuffer;
extern int attackCooldown;
extern long elapsedTime;
extern long loopStartTime;
void resetMovementVectors();
void setCycleStartPoints(int leg);
void setCycleStartPoints();
int angleToMicroseconds(double angle);
void moveToPos(int leg, Vector3 pos);

// ================================================================
// ===                         Bezier                           ===
// ================================================================
Vector2 GetPointOnBezierCurve(Vector2* points, int numPoints, float t);
Vector3 GetPointOnBezierCurve(Vector3* points, int numPoints, float t);
int binomialCoefficient(int n, int k);

// ================================================================
// ===                   Calibration_State                      ===
// ================================================================

void calibrationState();

// ================================================================
// ===                     Moving_State                         ===
// ================================================================
extern float forwardAmount;
extern float turnAmount;
extern float tArray[6];
extern int ControlPointsAmount;
extern int RotateControlPointsAmount;
extern float pushFraction;
extern float speedMultiplier;
extern float strideLengthMultiplier;
extern float liftHeightMultiplier;

extern float legPlacementAngle;

extern int leftSlider;
extern float globalSpeedMultiplier;
extern float globalRotationMultiplier;
extern float maxStrideLength;
extern float maxSpeed;

void movingState();
Vector3 getGaitPoint(int leg, float pushFraction);

// ================================================================
// ===                  Initialization_State                    ===
// ================================================================
void stateInitialize();
// ================================================================
// ===                      Standing_State                      ===
// ================================================================
void standingState();
void set3HighestLeg();

// ================================================================
// ===                       Printing                           ===
// ================================================================
void print_value(String name, float value, bool newLine);

// Hàm in giá trị kiểu String
void print_value(String name, String value, bool newLine);

// Hàm in giá trị kiểu Vector3
void print_value(String name, Vector3 value, bool newLine);


// ================================================================
// ===                      Initializations                     ===
// ================================================================
void attachServos();

// ================================================================
// ===                            Attacks                       ===
// ================================================================
void slamAttack();
Vector3 getFootPlacementPathPoint(int leg, float t);
Vector3 getLeapPathPoint(int leg, float t);
Vector3 getSlamPathPoint(int leg, float t);

// ================================================================
// ===                            Bluetooth                     ===
// ================================================================

#define rxBluetooth PA10  // Chân RX của STM32
#define txBluetooth PA9   // Chân TX của STM32

extern SoftwareSerial bluetoothSerial;
void Bluetooth_init();

#define rxPi PA3  // Chân RX của STM32
#define txPi PA2   // Chân TX của STM32

extern SoftwareSerial TalkPi;
void Serial_init();

#endif