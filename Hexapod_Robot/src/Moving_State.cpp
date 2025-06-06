#include <Arduino.h>
#include <header.h>
//================================================================
//                      Moving State
//================================================================


float forwardAmount;
float turnAmount;
float  tArray[6];
int ControlPointsAmount = 0;
int RotateControlPointsAmount = 0;
float pushFraction = 3.0/6.0;
float speedMultiplier = 0.5;
float strideLengthMultiplier = 1.5;
float liftHeightMultiplier = 1.0;
float maxStrideLength = 200;  //200
float maxSpeed = 100;
float legPlacementAngle = 45; //56

int leftSlider = 10;
float globalSpeedMultiplier = 0.55;   //tốc độ từ người dùng 0.55
float globalRotationMultiplier = 0.55; // độ xoay từ người dùng 0.55

bool finishedOneCycle = false;

void movingState() {
  rc_data.slider2 = 15; // 25
  leftSlider = (int)rc_data.slider2; 
  globalSpeedMultiplier = (leftSlider + 10.0)*0.01;
  globalRotationMultiplier = map(rc_data.slider2,0,100,40,130) * 0.01;

  if (currentState != Car || previousGait != currentGait) {
    currentState = Car;

    //Initialize Leg States
    for(int i = 0; i < 6; i++){
      legStates[i] = Reset;
    }   

    switch (currentGait) {
      case Tri:
        cycleProgress[0] = 0;
        cycleProgress[1] = (points / 2);
        cycleProgress[2] = 0;
        cycleProgress[3] = (points / 2);
        cycleProgress[4] = 0;
        cycleProgress[5] = (points / 2);

        pushFraction = 3.1/6.0;
        speedMultiplier = 1;
        strideLengthMultiplier = 1.2;
        liftHeightMultiplier = 1.1;
        maxStrideLength = 240;
        maxSpeed = 200;
        break;

      case Wave:
        //Offsets
        cycleProgress[0] = 0;
        cycleProgress[1] = (points / 6);
        cycleProgress[2] = (points / 6)*2;
        cycleProgress[3] = (points / 6)*5;
        cycleProgress[4] = (points / 6)*4;
        cycleProgress[5] = (points / 6)*3;

        //Percentage Time On Ground
        pushFraction = 5.0/6.0; 

        speedMultiplier = 0.40;
        strideLengthMultiplier = 2;
        liftHeightMultiplier = 1.3;
        maxStrideLength = 150;
        maxSpeed = 160;
        break;

      case Ripple:
        //Offsets
        cycleProgress[0] = 0;
        cycleProgress[1] = (points / 6)*4;
        cycleProgress[2] = (points / 6)*2;
        cycleProgress[3] = (points / 6)*5;
        cycleProgress[4] = (points / 6);
        cycleProgress[5] = (points / 6)*3;

        //Percentage Time On Ground
        pushFraction = 3.2/6.0;


        speedMultiplier = 1;
        strideLengthMultiplier = 1.3;
        liftHeightMultiplier = 1;
        maxStrideLength = 220;
        maxSpeed = 200;
        break;

      case Bi:
        //Offsets
        cycleProgress[0] = 0;
        cycleProgress[1] = (points / 3);
        cycleProgress[2] = (points / 3)*2;
        cycleProgress[3] = 0;
        cycleProgress[4] = (points / 3);
        cycleProgress[5] = (points / 3)*2;

        //Percentage Time On Ground
        pushFraction = 2.1/6.0;

        
        speedMultiplier = 4;        
        strideLengthMultiplier = 1;
        liftHeightMultiplier = 1.8;
        maxStrideLength = 230;
        maxSpeed = 130;
        break;

      case Quad:
        //Offsets
        cycleProgress[0] = 0;
        cycleProgress[1] = (points / 3);
        cycleProgress[2] = (points / 3)*2;
        cycleProgress[3] = 0;
        cycleProgress[4] = (points / 3);
        cycleProgress[5] = (points / 3)*2;

        //Percentage Time On Ground
        pushFraction = 4.1/6.0;

        
        speedMultiplier = 1;        
        strideLengthMultiplier = 1.2;
        liftHeightMultiplier = 1.8;
        maxStrideLength = 220;
        maxSpeed = 200;
        break;

      case Hop:
        //Offsets
        cycleProgress[0] = 0;
        cycleProgress[1] = 0;
        cycleProgress[2] = 0;
        cycleProgress[3] = 0;
        cycleProgress[4] = 0;
        cycleProgress[5] = 0;

        //Percentage Time On Ground        
        pushFraction = 3/6.0;

        speedMultiplier = 1;
        strideLengthMultiplier = 1.6;
        liftHeightMultiplier = 2.5;
        maxStrideLength = 240;
        maxSpeed = 200;
        break;
    }      
  }

  
  
  for(int i = 0; i < 6; i++){
    tArray[i] = (float)cycleProgress[i] / points;    
  }  

  forwardAmount = joy1CurrentMagnitude;
  turnAmount = joy2CurrentVector.x;

  moveToPos(0, getGaitPoint(0, pushFraction));
  moveToPos(1, getGaitPoint(1, pushFraction));
  moveToPos(2, getGaitPoint(2, pushFraction));
  moveToPos(3, getGaitPoint(3, pushFraction));
  moveToPos(4, getGaitPoint(4, pushFraction));
  moveToPos(5, getGaitPoint(5, pushFraction));
  
  

  float progressChangeAmount = (max(abs(forwardAmount),abs(turnAmount))* speedMultiplier)*globalSpeedMultiplier ;

  
  progressChangeAmount = constrain(progressChangeAmount,0,maxSpeed*globalSpeedMultiplier);

  for(int i = 0; i < 6; i++){
    cycleProgress[i] += progressChangeAmount;

    if(cycleProgress[i] >= points){
      cycleProgress[i] = cycleProgress[i] - points;
    }
  } 
}


Vector3 getGaitPoint(int leg, float pushFraction){  
  float rotateStrideLength = joy2CurrentVector.x * globalRotationMultiplier;
  Vector2 v = joy1CurrentVector * Vector2(1,strideLengthMultiplier);
  v.y = constrain(v.y,-maxStrideLength/2, maxStrideLength/2);
  v = v * globalSpeedMultiplier;

  float weightSum = abs(forwardAmount) + abs(turnAmount);

  float t = tArray[leg];

  //if(leg == 0)print_value("cycleProgress[leg]",cycleProgress[leg]);
  
  
  //Propelling
  if(t < pushFraction){ 
    if(legStates[leg] != Propelling)setCycleStartPoints(leg);
    legStates[leg] = Propelling;
    if (leg == 0 || leg == 2 || leg == 3 || leg == 5) {
      distanceFromCenter = 65;
  }
    ControlPoints[0] = cycleStartPoints[leg];
    ControlPoints[1] = Vector3(v.x * strideMultiplier[leg] + distanceFromCenter, 
                              -v.y * strideMultiplier[leg], 
                              distanceFromGround).rotate(legPlacementAngle * rotationMultiplier[leg], Vector2(distanceFromCenter,0));
    ControlPointsAmount = 2;    
    Vector3 straightPoint = GetPointOnBezierCurve(ControlPoints, ControlPointsAmount, mapFloat(t,0,pushFraction,0,1));

    RotateControlPoints[0] = cycleStartPoints[leg];
    RotateControlPoints[1] = { distanceFromCenter + 0, 0, distanceFromGround };
    RotateControlPoints[2] = { distanceFromCenter, rotateStrideLength, distanceFromGround };
    RotateControlPointsAmount = 3;    
    Vector3 rotatePoint = GetPointOnBezierCurve(RotateControlPoints, RotateControlPointsAmount, mapFloat(t,0,pushFraction,0,1));

    //if(leg == 0)print_value("pushing point",(straightPoint*abs(forwardAmount) + rotatePoint*abs(turnAmount))/ weightSum);

    return (straightPoint*abs(forwardAmount) + rotatePoint*abs(turnAmount))/ weightSum;
  }

  //Lifting
  else{
    if(legStates[leg] != Lifting)setCycleStartPoints(leg);
    legStates[leg] = Lifting;
    
    if (leg == 0 || leg == 2 || leg == 3 || leg == 5) {
      distanceFromCenter = 65;
  }
    ControlPoints[0] = cycleStartPoints[leg];
    ControlPoints[1] = cycleStartPoints[leg] + Vector3(0,0,liftHeight * liftHeightMultiplier);
    ControlPoints[2] = Vector3(-v.x * strideMultiplier[leg] + distanceFromCenter, (v.y + strideOvershoot) * strideMultiplier[leg], distanceFromGround + landHeight).rotate(legPlacementAngle * rotationMultiplier[leg], Vector2(distanceFromCenter,0));
    ControlPoints[3] = Vector3(-v.x * strideMultiplier[leg] + distanceFromCenter, v.y * strideMultiplier[leg], distanceFromGround).rotate(legPlacementAngle * rotationMultiplier[leg], Vector2(distanceFromCenter,0));
    ControlPointsAmount = 4;
    Vector3 straightPoint = GetPointOnBezierCurve(ControlPoints, ControlPointsAmount, mapFloat(t,pushFraction,1,0,1));

    RotateControlPoints[0] = cycleStartPoints[leg];
    RotateControlPoints[1] = cycleStartPoints[leg] + Vector3(0,0,liftHeight * liftHeightMultiplier);
    RotateControlPoints[2] = { distanceFromCenter + 0, 0, distanceFromGround + liftHeight * liftHeightMultiplier};
    RotateControlPoints[3] = { distanceFromCenter, -(rotateStrideLength + strideOvershoot), distanceFromGround + landHeight};
    RotateControlPoints[4] = { distanceFromCenter, -rotateStrideLength, distanceFromGround};
    RotateControlPointsAmount = 5;
    Vector3 rotatePoint =  GetPointOnBezierCurve(RotateControlPoints, RotateControlPointsAmount, mapFloat(t,pushFraction,1,0,1));

    //if(leg == 0)print_value("lifting point",(straightPoint*abs(forwardAmount) + rotatePoint*abs(turnAmount))/ weightSum);

    return (straightPoint*abs(forwardAmount) + rotatePoint*abs(turnAmount))/ weightSum;
  }  
}

// Initialize
void stateInitialize(){
  // moveToPos(0, Vector3(a1+a2+a3,0,0));
  // moveToPos(1, Vector3(a1+a2+a3,0,0));
  // moveToPos(2, Vector3(a1+a2+a3,0,0));
  // moveToPos(3, Vector3(a1+a2+a3,0,0));
  // moveToPos(4, Vector3(a1+a2+a3,0,0));
  // moveToPos(5, Vector3(a1+a2+a3,0,0));

  // delay(1000);

  moveToPos(0, Vector3(a1+a3,0,a2));
  moveToPos(1, Vector3(a1+a3,0,a2));
  moveToPos(2, Vector3(a1+a3,0,a2));
  moveToPos(3, Vector3(a1+a3,0,a2));
  moveToPos(4, Vector3(a1+a3,0,a2));
  moveToPos(5, Vector3(a1+a3,0,a2));
 
  delay(500);
}


void resetMovementVectors(){
  joy1CurrentVector = Vector2(0,0);
  joy1CurrentMagnitude = 0;

  joy2CurrentVector = Vector2(0,0);
  joy2CurrentMagnitude = 0;
}

void setCycleStartPoints(int leg){
  cycleStartPoints[leg] = currentPoints[leg];    
}

void setCycleStartPoints(){
  for(int i = 0; i < 6; i++){
    cycleStartPoints[i] = currentPoints[i]; 
  }     
}

int angleToMicroseconds(double angle) {
  angle = constrain(angle, 0, 180);
  double val = 500.0 + (((2500.0 - 500.0) / 180.0) * angle);
  return (int)val;
}

void rotateToAngle(int leg, Vector3 targetRot) {
  attachServos();  // Kiểm tra xem servo đã được gắn chưa

  // Chuyển đổi góc sang microseconds
  int coxaMicroseconds = angleToMicroseconds(targetRot.x);
  int femurMicroseconds = angleToMicroseconds(targetRot.y);
  int tibiaMicroseconds = angleToMicroseconds(targetRot.z);

  // Sử dụng servo driver để điều khiển các servo cho mỗi chân
  switch (leg) {
    case 0:
      servoDriver_0.setPWM(coxa1Pin, 0, microsecondsToPWM(coxaMicroseconds));
      servoDriver_0.setPWM(femur1Pin, 0, microsecondsToPWM(femurMicroseconds));
      servoDriver_0.setPWM(tibia1Pin, 0, microsecondsToPWM(tibiaMicroseconds));
      break;

    case 1:
      servoDriver_0.setPWM(coxa2Pin, 0, microsecondsToPWM(coxaMicroseconds));
      servoDriver_0.setPWM(femur2Pin, 0, microsecondsToPWM(femurMicroseconds));
      servoDriver_0.setPWM(tibia2Pin, 0, microsecondsToPWM(tibiaMicroseconds));
      break;

    case 2:
      servoDriver_0.setPWM(coxa3Pin, 0, microsecondsToPWM(coxaMicroseconds));
      servoDriver_0.setPWM(femur3Pin, 0, microsecondsToPWM(femurMicroseconds));
      servoDriver_0.setPWM(tibia3Pin, 0, microsecondsToPWM(tibiaMicroseconds));
      break;

    case 3:
      servoDriver_1.setPWM(coxa4Pin, 0, microsecondsToPWM(coxaMicroseconds));
      servoDriver_1.setPWM(femur4Pin, 0, microsecondsToPWM(femurMicroseconds));
      servoDriver_1.setPWM(tibia4Pin, 0, microsecondsToPWM(tibiaMicroseconds));
      break;

    case 4:
      servoDriver_1.setPWM(coxa5Pin, 0, microsecondsToPWM(coxaMicroseconds));
      servoDriver_1.setPWM(femur5Pin, 0, microsecondsToPWM(femurMicroseconds));
      servoDriver_1.setPWM(tibia5Pin, 0, microsecondsToPWM(tibiaMicroseconds));
      break;

    case 5:
      servoDriver_1.setPWM(coxa6Pin, 0, microsecondsToPWM(coxaMicroseconds));
      servoDriver_1.setPWM(femur6Pin, 0, microsecondsToPWM(femurMicroseconds));
      servoDriver_1.setPWM(tibia6Pin, 0, microsecondsToPWM(tibiaMicroseconds));
      break;

    default:
      break;
  }
  return;
}

void moveToPos(int leg, Vector3 pos){
  currentPoints[leg] = pos;
  
  float dis = Vector3(0,0,0).distanceTo(pos);
  if(dis > legLength){
    print_value("Point impossible to reach", pos, false);
    print_value("Distance",dis, true);
    return;
  }

  float x = pos.x;
  float y = pos.y;
  float z = pos.z;

  float o1 = offsets[leg].x;
  float o2 = offsets[leg].y;
  float o3 = offsets[leg].z;

  // Xử lý mirroring cho chân 3, 4, 5
  bool isMirrored = (leg == 3 || leg == 4 || leg == 5);

  float theta1 = atan2(y, x) * (180 / PI) + o1; // Base angle
  float l = sqrt(x * x + y * y);               // X and Y extension
  float l1 = l - a1;
  float h = sqrt(l1 * l1 + z * z);

  float phi1 = acos(constrain((pow(h, 2) + pow(a2, 2) - pow(a3, 2)) / (2 * h * a2), -1, 1));
  float phi2 = atan2(z, l1);
  float theta2 = (phi1 + phi2) * 180 / PI + o2;

  float phi3 = acos(constrain((pow(a2, 2) + pow(a3, 2) - pow(h, 2)) / (2 * a2 * a3), -1, 1));
  float theta3 = 180 - (phi3 * 180 / PI) + o3;

  // Nếu chân mirrored, thay đổi cách tính góc theta2 và theta3
  if (isMirrored) {
      theta2 = -theta2 + 2 * o2; // Mirror góc theta2
      theta3 = -theta3 + 2 * o3; // Mirror góc theta3
  }

  targetRot = Vector3(theta1,theta2,theta3);
  
  int coxaMicroseconds = angleToMicroseconds(targetRot.x);
  int femurMicroseconds = angleToMicroseconds(targetRot.y);
  int tibiaMicroseconds = angleToMicroseconds(targetRot.z);

  switch(leg){
    case 0:
      servoDriver_0.setPWM(coxa1Pin, 0, microsecondsToPWM(coxaMicroseconds));
      servoDriver_0.setPWM(femur1Pin, 0, microsecondsToPWM(femurMicroseconds));
      servoDriver_0.setPWM(tibia1Pin, 0, microsecondsToPWM(tibiaMicroseconds));
      break;

    case 1:
      servoDriver_0.setPWM(coxa2Pin, 0, microsecondsToPWM(coxaMicroseconds));
      servoDriver_0.setPWM(femur2Pin, 0, microsecondsToPWM(femurMicroseconds));
      servoDriver_0.setPWM(tibia2Pin, 0, microsecondsToPWM(tibiaMicroseconds));
      break;

    case 2:
      servoDriver_0.setPWM(coxa3Pin, 0, microsecondsToPWM(coxaMicroseconds));
      servoDriver_0.setPWM(femur3Pin, 0, microsecondsToPWM(femurMicroseconds));
      servoDriver_0.setPWM(tibia3Pin, 0, microsecondsToPWM(tibiaMicroseconds));
      break;

    case 3:
      servoDriver_1.setPWM(coxa4Pin, 0, microsecondsToPWM(coxaMicroseconds));
      servoDriver_1.setPWM(femur4Pin, 0, microsecondsToPWM(femurMicroseconds));
      servoDriver_1.setPWM(tibia4Pin, 0, microsecondsToPWM(tibiaMicroseconds));
      break;

    case 4:
      servoDriver_1.setPWM(coxa5Pin, 0, microsecondsToPWM(coxaMicroseconds));
      servoDriver_1.setPWM(femur5Pin, 0, microsecondsToPWM(femurMicroseconds));
      servoDriver_1.setPWM(tibia5Pin, 0, microsecondsToPWM(tibiaMicroseconds));
      break;

    case 5:
      servoDriver_1.setPWM(coxa6Pin, 0, microsecondsToPWM(coxaMicroseconds));
      servoDriver_1.setPWM(femur6Pin, 0, microsecondsToPWM(femurMicroseconds));
      servoDriver_1.setPWM(tibia6Pin, 0, microsecondsToPWM(tibiaMicroseconds));
      break;

    default:
      break;
  }
  return; 
}

Vector3 calculateIK(int leg, Vector3 pos) {
  float dis = Vector3(0,0,0).distanceTo(pos);
  if(dis > legLength){
    print_value("Point impossible to reach", pos, false);
    print_value("Distance", dis, true);
    return Vector3(0, 0, 0); // hoặc bạn có thể trả về giá trị đặc biệt để báo lỗi
  }

  float x = pos.x;
  float y = pos.y;
  float z = pos.z;

  float o1 = offsets[leg].x;
  float o2 = offsets[leg].y;
  float o3 = offsets[leg].z;

  bool isMirrored = (leg == 3 || leg == 4 || leg == 5);

  float theta1 = atan2(y, x) * (180 / PI) + o1;
  float l = sqrt(x * x + y * y);
  float l1 = l - a1;
  float h = sqrt(l1 * l1 + z * z);

  float phi1 = acos(constrain((pow(h, 2) + pow(a2, 2) - pow(a3, 2)) / (2 * h * a2), -1, 1));
  float phi2 = atan2(z, l1);
  float theta2 = (phi1 + phi2) * 180 / PI + o2;

  float phi3 = acos(constrain((pow(a2, 2) + pow(a3, 2) - pow(h, 2)) / (2 * a2 * a3), -1, 1));
  float theta3 = 180 - (phi3 * 180 / PI) + o3;

  if (isMirrored) {
    theta2 = -theta2 + 2 * o2;
    theta3 = -theta3 + 2 * o3;
  }

  return Vector3(theta1, theta2, theta3);
}
bool gaitCycleCompleted = false;

void debugMovingState() {
  static int completedCycleCount = 0;
  static int initialCycleOffset[6] = {0};  // Lưu offset ban đầu

  if (completedCycleCount >= 4) return;
  rc_data.slider1 = 0;
  rc_data.slider2 = 25;
  leftSlider = (int)rc_data.slider2; 
  globalSpeedMultiplier = (leftSlider + 10.0) * 0.01;
  globalRotationMultiplier = map(rc_data.slider2, 0, 100, 40, 130) * 0.01;

  if (currentState != Car || previousGait != currentGait) {
    currentState = Car;
    completedCycleCount = 0;

    for (int i = 0; i < 6; i++) {
      legStates[i] = Reset;
    }

    switch (currentGait) {
      case Tri:
        initialCycleOffset[0] = 0;
        initialCycleOffset[1] = points / 2;
        initialCycleOffset[2] = 0;
        initialCycleOffset[3] = points / 2;
        initialCycleOffset[4] = 0;
        initialCycleOffset[5] = points / 2;
        pushFraction = 3.1/6.0;
        speedMultiplier = 1;
        strideLengthMultiplier = 1.2;
        liftHeightMultiplier = 1.1;
        maxStrideLength = 240;
        maxSpeed = 200;
        break;

      case Wave:
        initialCycleOffset[0] = 0;
        initialCycleOffset[1] = points / 6;
        initialCycleOffset[2] = (points / 6) * 2;
        initialCycleOffset[3] = (points / 6) * 5;
        initialCycleOffset[4] = (points / 6) * 4;
        initialCycleOffset[5] = (points / 6) * 3;
        pushFraction = 5.0/6.0;
        speedMultiplier = 0.40;
        strideLengthMultiplier = 2;
        liftHeightMultiplier = 1.3;
        maxStrideLength = 150;
        maxSpeed = 160;
        break;

      case Ripple:
        initialCycleOffset[0] = 0;
        initialCycleOffset[1] = (points / 6) * 4;
        initialCycleOffset[2] = (points / 6) * 2;
        initialCycleOffset[3] = (points / 6) * 5;
        initialCycleOffset[4] = (points / 6);
        initialCycleOffset[5] = (points / 6) * 3;
        pushFraction = 3.2/6.0;
        speedMultiplier = 1;
        strideLengthMultiplier = 1.3;
        liftHeightMultiplier = 1;
        maxStrideLength = 220;
        maxSpeed = 200;
        break;

      case Bi:
        initialCycleOffset[0] = 0;
        initialCycleOffset[1] = points / 3;
        initialCycleOffset[2] = (points / 3) * 2;
        initialCycleOffset[3] = 0;
        initialCycleOffset[4] = points / 3;
        initialCycleOffset[5] = (points / 3) * 2;
        pushFraction = 2.1/6.0;
        speedMultiplier = 4;
        strideLengthMultiplier = 1;
        liftHeightMultiplier = 1.8;
        maxStrideLength = 230;
        maxSpeed = 130;
        break;

      case Quad:
        initialCycleOffset[0] = 0;
        initialCycleOffset[1] = points / 3;
        initialCycleOffset[2] = (points / 3) * 2;
        initialCycleOffset[3] = 0;
        initialCycleOffset[4] = points / 3;
        initialCycleOffset[5] = (points / 3) * 2;
        pushFraction = 4.1/6.0;
        speedMultiplier = 1;
        strideLengthMultiplier = 1.2;
        liftHeightMultiplier = 1.8;
        maxStrideLength = 220;
        maxSpeed = 200;
        break;

      case Hop:
        for (int i = 0; i < 6; i++) initialCycleOffset[i] = 0;
        pushFraction = 3.0 / 6.0;
        speedMultiplier = 1;
        strideLengthMultiplier = 1.6;
        liftHeightMultiplier = 2.5;
        maxStrideLength = 240;
        maxSpeed = 200;
        break;
    }

    // Sau khi đặt offset, gán cho cycleProgress
    for (int i = 0; i < 6; i++) {
      cycleProgress[i] = initialCycleOffset[i];
    }
  }

  forwardAmount = joy1CurrentMagnitude;
  turnAmount = joy2CurrentVector.x;

  bool allLegsCompleted = true;

  for (int i = 0; i < 6; i++) {
    tArray[i] = (float)cycleProgress[i] / points;
    Vector3 gaitPos = getGaitPoint(i, pushFraction);
    Vector3 jointAngles = calculateIK(i, gaitPos);

    TalkPi.print(i); TalkPi.print(",");
    TalkPi.print(cycleProgress[i] + completedCycleCount * points); TalkPi.print(",");
    TalkPi.print(gaitPos.x); TalkPi.print(",");
    TalkPi.print(gaitPos.y); TalkPi.print(",");
    TalkPi.print(gaitPos.z); TalkPi.print(",");
    TalkPi.print(jointAngles.x); TalkPi.print(",");
    TalkPi.print(jointAngles.y); TalkPi.print(",");
    TalkPi.println(jointAngles.z);

    if (cycleProgress[i] < points) allLegsCompleted = false;
  }

  if (!allLegsCompleted) {
    float progressChangeAmount = max(abs(forwardAmount), abs(turnAmount)) * speedMultiplier * globalSpeedMultiplier;
    progressChangeAmount = constrain(progressChangeAmount, 0, maxSpeed * globalSpeedMultiplier);
    for (int i = 0; i < 6; i++) {
      cycleProgress[i] += progressChangeAmount;
      if (cycleProgress[i] > points) cycleProgress[i] = points;
    }
  } else {
    completedCycleCount++;
    for (int i = 0; i < 6; i++) {
      cycleProgress[i] = initialCycleOffset[i];  // Giữ lại phase ban đầu
    }
    TalkPi.print("=== Gait cycle complete: ");
    TalkPi.print(completedCycleCount);
    TalkPi.println(" ===");
  }
}





