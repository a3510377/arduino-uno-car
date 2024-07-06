#include <Arduino.h>
#include <UnoCar.h>

#define SENSOR_COUNT 8

UnoCar uno_car(SENSOR_COUNT);

void setup() {
  Serial.begin(9600);

  uno_car.init();  // 初始化 UnoCar
}

void loop() {
  for (int i = 0; i < 8; i++) {
    int value = uno_car.get(i);
    Serial.print(value);  // 輸出感測器數據

    if (i != 7) Serial.print(",");

    // 檢查是否超過閥值
    if (value < 300) {
      uno_car.leds[i] = HIGH;  // 關閉 LED
    } else {
      uno_car.leds[i] = LOW;  // 開啟LED
    }
  }
  Serial.println();
}
