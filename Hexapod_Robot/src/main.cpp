#include <Arduino.h>
#include <header.h>

//==============================================
//==================CONST=======================
//==============================================

const float a1 = 31;  //Coxa Length
const float a2 = 70; //Femur Length
const float a3 = 130; //Tibia Length   
float legLength = a1 + a2 + a3;

State currentState = Initialize;
Gait currentGait = Tri;
Gait previousGait = Tri;
int currentGaitID = 0;

int totalGaits = 6;
Gait gaits[6] = {Tri,Wave,Ripple,Bi,Quad,Hop};


float points = 1000;
int cycleProgress[6];
LegState legStates[6];
int standProgress = 0;

float standingDistanceAdjustment = 0;

float distanceFromGroundBase = -60;
float distanceFromGround = 0; 
float previousDistanceFromGround = 0;

float liftHeight = 130;           //Độ cao nhấc chân
float landHeight = 70;
float strideOvershoot = 10;
float distanceFromCenter = 190;         //Khoảng cách đến tâm

float crabTargetForwardAmount = 0;
float crabForwardAmount = 0;

Vector2 joy1TargetVector = Vector2(0,0);
float joy1TargetMagnitude = 0;
Vector2 joy1CurrentVector = Vector2(0,0);
float joy1CurrentMagnitude = 0;
Vector2 joy2TargetVector = Vector2(0,0);
float joy2TargetMagnitude = 0;
Vector2 joy2CurrentVector = Vector2(0,0);
float joy2CurrentMagnitude = 0;

unsigned long timeSinceLastInput = 0;

float landingBuffer = 15;

int attackCooldown = 0;
long elapsedTime = 0;
long loopStartTime = 0;

//===================================================

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
  attachServos(); //Init Servo
  RC_Setup();
  stateInitialize();

  rc_data.joy1_X = 127;
  rc_data.joy1_Y = 127;
  rc_data.joy2_X = 200;
  rc_data.joy2_Y = 200;
}

void loop() {
  // for (uint16_t pulse = 150; pulse < 500; pulse += 10) {
  //   servoDriver_0.setPWM(0, 0, pulse); // Kênh 0 trên PCA1
  //   servoDriver_1.setPWM(0, 0, pulse); // Kênh 1 trên PCA2
  //   delay(100);               // Đợi một chút để quan sát
  // }
  // delay(500);

  // for (uint16_t pulse = 500; pulse > 150; pulse -= 10) {
  //   servoDriver_0.setPWM(0, 0, pulse); // Kênh 0 trên PCA1
  //   servoDriver_1.setPWM(0, 0, pulse); // Kênh 1 trên PCA2
  //   delay(100);               // Đợi một chút để quan sát
  // }
  // delay(500);
  elapsedTime = millis() - loopStartTime;
  loopStartTime = millis();

  bool connected = GetData();
  //RC_DisplayData();
  // if (rc_data.joy1_X < 240){
  //   rc_data.joy1_X += 10;
  //   rc_data.joy1_Y += 10;
  //   rc_data.joy2_X += 10;
  //   rc_data.joy2_Y += 10;
  // }
  // else {
  //   rc_data.joy1_X = 197;
  //   rc_data.joy1_Y = 197;
  //   rc_data.joy2_X = 127;
  //   rc_data.joy2_Y = 127;
  // }
  if(connected){
    // Data 0 -> 254, Default: 127, Scale into -100 -> 100
    double joy1x = map(rc_data.joy1_X,0,254,-100,100);
    double joy1y = map(rc_data.joy1_Y,0,254,-100,100);

    double joy2x = map(rc_data.joy2_X,0,254,-100,100);
    double joy2y = map(rc_data.joy2_Y,0,254,-100,100);
    
    joy1TargetVector = Vector2(joy1x,joy1y);
    joy1TargetMagnitude = constrain(calculateHypotenuse(abs(joy1x),abs(joy1y)),0,100);   

    joy2TargetVector = Vector2(joy2x,joy2y);
    joy2TargetMagnitude = constrain(calculateHypotenuse(abs(joy2x),abs(joy2y)),0,100);  

    previousDistanceFromGround = distanceFromGround;
    distanceFromGround = distanceFromGroundBase + rc_data.slider1 * -1.7;
    distanceFromCenter = 170;

  
  }
  else{
    calibrationState();
    //Serial.println("State: Disconnected");
    return;
  }
  // tốc độ nội suy 8%
  joy1CurrentVector = lerp(joy1CurrentVector, joy1TargetVector, 0.08);
  joy1CurrentMagnitude = lerp(joy1CurrentMagnitude, joy1TargetMagnitude, 0.08);
  // tốc độ nội suy 12%
  joy2CurrentVector = lerp(joy2CurrentVector, joy2TargetVector, 0.12);
  joy2CurrentMagnitude = lerp(joy2CurrentMagnitude, joy2TargetMagnitude, 0.12);  

  //======Change gait===========
  previousGait = currentGait;
  if(rc_data.pushButton2 == 1  && rc_data_previous.pushButton2 == 0){ 
    currentGaitID += 1;
    if(currentGaitID == totalGaits){
      currentGaitID = 0;
    }    
    
    currentGait = gaits[currentGaitID];
  }
  //============================
  
  //=========Attack=============
  if(rc_data.joy1_Button == 1 && attackCooldown == 0){
    Serial.println("slam attack");
    resetMovementVectors();
    slamAttack();
    standingState();
    attackCooldown = 50;    
    loopStartTime = millis();
    return;
  }
  //============================
  
  else{
    attackCooldown = max(attackCooldown - elapsedTime, (long) 0);
  }

  //=========Moving=============
  if(abs(joy1CurrentMagnitude) >= 10 || abs(joy2CurrentMagnitude) >= 10){
    movingState();
    timeSinceLastInput = millis();
    return;
  }
  //============================
  //==========Stand=============
  if(abs((long) timeSinceLastInput - (long) millis()) >(long) 5) {
    standingState();
    return;
  }  
  //============================
}








