#include <Arduino.h>
#include <header.h>
Vector3 targetCalibration = Vector3(a1+a3,0,a2);
int inBetweenZ = -20;

void calibrationState(){
  currentState = Calibrate;

  bool legsUp = true;

  for (int i = 0; i < 6; i++){
    if(currentPoints[i].z < inBetweenZ) legsUp = false;
  }

  if(!legsUp){
    for (int i = 0; i < 6; i++){
      float nextZ = lerp(currentPoints[i].z, inBetweenZ + 2, 0.02);
      moveToPos(i, Vector3(currentPoints[i].x,currentPoints[i].y,nextZ));  
    }
  }
  else{
    for (int i = 0; i < 6; i++){
      float nextX = min(currentPoints[i].x + 0.5f, targetCalibration.x);
      float nextY = min(currentPoints[i].y + 0.5f, targetCalibration.y);
      float nextZ = min(currentPoints[i].z + 0.5f, targetCalibration.z);
      moveToPos(i, Vector3(nextX,nextY,nextZ));
    }
  }
}