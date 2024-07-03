#include <Arduino.h>
#include <UnoCar.h>

#define SENSOR_COUNT 8

UnoCar uno_car(SENSOR_COUNT);

void setup() {
  Serial.begin(9600);

  uno_car.init();  // 初始化 UnoCar
}

void loop() {
  uno_car.back();  // 後退速度 255
  delay(1000);
  uno_car.back(255, 100);  // 後退 A輪255, B輪100
  delay(1000);

  uno_car.forward();  // 前進速度 255
  delay(1000);
  uno_car.forward(100);  // 前進速度 100
  delay(1000);

  uno_car.stopA();  // 關閉 A 輪
  delay(1000);
  uno_car.stopB();  // 關閉 B 輪
  delay(1000);

  uno_car.drive(100);  // 兩輪前進速度 100
  delay(1000);
  uno_car.drive(100, -100);  // A 輪前進速度 100, B 輪後退速度 100
  delay(1000);
  uno_car.drive(100, -100);  // A 輪前進速度 100, B 輪後退速度 100
  delay(1000);

  uno_car.driveA(100);  // A 輪前進速度 100
  delay(1000);
  uno_car.driveB(100);  // B 輪前進速度 100
  delay(1000);
}
