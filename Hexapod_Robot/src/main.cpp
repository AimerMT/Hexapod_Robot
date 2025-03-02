#include <Arduino.h>
#include <header.h>

//==============================================
//==================CONST=======================
//==============================================

const float a1 = 29;  //Coxa Length
const float a2 = 76; //Femur Length
const float a3 = 106; //Tibia Length   
float legLength = a1 + a2 + a3;

State currentState = Initialize;
Gait currentGait = Ripple;
Gait previousGait = Ripple;
int currentGaitID = 0;

int totalGaits = 6;
Gait gaits[6] = {Tri,Wave,Ripple,Bi,Quad,Hop};


float points = 1000;
int cycleProgress[6];
LegState legStates[6];
int standProgress = 0;

float standingDistanceAdjustment = 0; //-60   // khoảng cách thân so với đất trong standingstate 

float distanceFromGroundBase = -60;   //-60   // khoảng cách thân so với đất trong movingstate 
float distanceFromGround = 0; 
float previousDistanceFromGround = 0;

float liftHeight = 40;    //130       
float landHeight = 20;     //70
float strideOvershoot = 10;
float distanceFromCenter = 50;         //Khoảng cách đến tâm

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
  delay(1000);
  //standingState();
  rc_data.joy1_X = 127; // Đi ngang
  rc_data.joy1_Y = 200; // Tiến + Lùi
  rc_data.joy2_X = 127; // 127->255 : Xoay trái ; 0->127 : Xoay phải
  rc_data.joy2_Y = 127;
}

void loop() {

  elapsedTime = millis() - loopStartTime;
  loopStartTime = millis();

  bool connected = true;
  if(connected){
    // Data 0 -> 254, Default: 127, Scale into -100 -> 100
    double joy1x = map(rc_data.joy1_X,0,254,-100,100);
    double joy1y = map(rc_data.joy1_Y,0,254,-100,100);

    double joy2x = map(rc_data.joy2_X,0,254,-100,100);
    double joy2y = map(rc_data.joy2_Y,0,254,-100,100);
    
    joy1TargetVector = Vector2(joy1x,joy1y);
    joy1TargetMagnitude = constrain(calculateHypotenuse(abs(joy1x),abs(joy1y)),0,100);    // giới hạn giá trị độ lớn tính được trong khoảng 0-100

    joy2TargetVector = Vector2(joy2x,joy2y);
    joy2TargetMagnitude = constrain(calculateHypotenuse(abs(joy2x),abs(joy2y)),0,100);  

    previousDistanceFromGround = distanceFromGround;
    distanceFromGround = distanceFromGroundBase + rc_data.slider1 * -1.7;
    distanceFromCenter = 100; //78

  
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








