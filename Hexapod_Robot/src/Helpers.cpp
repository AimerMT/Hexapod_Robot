#include <Arduino.h>
#include <header.h>


// ======================
// Nội suy tuyến tính, a là bắt đầu, b là kết thúc, f là hệ số nội suy

float lerp(float a, float b, float f)
{
    return a * (1.0 - f) + (b * f);
}

Vector2 lerp(Vector2 a, Vector2 b, float f)
{
    return Vector2(lerp(a.x, b.x, f), lerp(a.y, b.y, f));
}

float calculateHypotenuse(float x, float y) {
  float result = sqrt(pow(x, 2) + pow(y, 2));
  return result;
}


float mapFloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// void switchUART(bool useBT) {
//   if (useBT) {
//       TalkPi.end();      // Tắt UART1 (Raspberry Pi)
//       bluetoothSerial.begin(9600); // Bật UART2 (HC-05)
//   } else {
//     bluetoothSerial.end();     // Tắt UART2 (HC-05)
//     TalkPi.begin(9600); // Bật UART1 (Raspberry Pi)
//   }
//   useBluetooth = useBT;
// }
