#include <Arduino.h>
#include <UnoCar.h>

#define SENSOR_COUNT 8

UnoCar uno_car(SENSOR_COUNT);

void setup() {
  Serial.begin(9600);

  uno_car.init();  // 初始化 UnoCar
}

void loop() {
  if (uno_car.isTouch()) {  // 判斷是否觸碰
    Serial.println("Touch");
  }
  delay(20);
}
