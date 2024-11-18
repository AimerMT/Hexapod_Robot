#pragma once
#include "vectors.h"
#include <Arduino.h>

float lerp(float a, float b, float f);


Vector2 lerp(Vector2 a, Vector2 b, float f);


float calculateHypotenuse(float x, float y);


float mapFloat(float x, float in_min, float in_max, float out_min, float out_max);
