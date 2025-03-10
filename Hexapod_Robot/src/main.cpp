#include <Arduino.h>
#include <header.h>
#include <RC.h>

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
#define DATA_LENGTH 5 // Số byte dữ liệu nhận (0, Ball1.X, Ball1.Y, Ball2.X, Ball2.Y)

bool readData = false;
int indexOfCurDataByte = 0;
uint8_t Data[DATA_LENGTH];

//===================================================

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
  attachServos(); //Init Servo
  Bluetooth_init();
  RC_Setup();
  stateInitialize();
  delay(1000);
  //standingState();
  rc_data.joy1_X = 127; // Đi ngang
  rc_data.joy1_Y = 127; // Tiến + Lùi
  rc_data.joy2_X = 127; // 127->255 : Xoay trái ; 0->127 : Xoay phải
  rc_data.joy2_Y = 127;
}

void loop() {

  elapsedTime = millis() - loopStartTime;
  loopStartTime = millis();

  while (bluetoothSerial.available()) {
    if (!readData) {
        if (bluetoothSerial.read() == 0) { // Xác định byte bắt đầu
            readData = true;
            indexOfCurDataByte = 0;
        }
    } else {
        int data = bluetoothSerial.read();

        if (indexOfCurDataByte >= DATA_LENGTH || data == 0) { // Kết thúc gói tin
            readData = false;
            indexOfCurDataByte = 0;
        }

        Data[indexOfCurDataByte] = data;
        indexOfCurDataByte++;
    }
}

if (indexOfCurDataByte == DATA_LENGTH) { // Đảm bảo đọc đủ dữ liệu trước khi xử lý
    rc_data.joy1_X = constrain(map(Data[0], 20, 180, 0, 255), 0, 255);  // Ball1.X
    rc_data.joy1_Y = constrain(map(Data[1], 20, 180, 255, 0), 0, 255);  // Ball1.Y
    rc_data.joy2_X = constrain(map(Data[2], 20, 180, 0, 255), 0, 255);  // Ball2.X
    rc_data.joy2_Y = constrain(map(Data[3], 20, 180, 255, 0), 0, 255);  // Ball2.Y

    double joy1x = map(rc_data.joy1_X, 0, 255, -100, 100);
    double joy1y = map(rc_data.joy1_Y, 0, 255, -100, 100);
    double joy2x = map(rc_data.joy2_X, 0, 255, -100, 100);
    double joy2y = map(rc_data.joy2_Y, 0, 255, -100, 100);

    joy1TargetVector = Vector2(joy1x, joy1y);
    joy1TargetMagnitude = constrain(calculateHypotenuse(abs(joy1x), abs(joy1y)), 0, 100);

    joy2TargetVector = Vector2(joy2x, joy2y);
    joy2TargetMagnitude = constrain(calculateHypotenuse(abs(joy2x), abs(joy2y)), 0, 100);

    previousDistanceFromGround = distanceFromGround;
    distanceFromGround = distanceFromGroundBase + rc_data.slider1 * -1.7;
    distanceFromCenter = 100;
} else {
    calibrationState();
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
  /*if(rc_data.pushButton2 == 1  && rc_data_previous.pushButton2 == 0){ 
    currentGaitID += 1;
    if(currentGaitID == totalGaits){
      currentGaitID = 0;
    }    
    
    currentGait = gaits[currentGaitID];
  }*/
  //============================
  
  //=========Attack=============
  /*if(rc_data.joy1_Button == 1 && attackCooldown == 0){
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
*/
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








