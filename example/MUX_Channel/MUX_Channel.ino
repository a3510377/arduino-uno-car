#include <Arduino.h>
#include <UnoCar.h>

#define SENSOR_COUNT 8

UnoCar uno_car(SENSOR_COUNT);

void setup() {
  Serial.begin(9600);

  uno_car.init();  // 初始化 UnoCar
}

void loop() {
  if (uno_car.update_mux()) {  // 讀取感測器數據
    for (int i = 0; i < 8; i++) {
      Serial.print(uno_car.get(i));  // 輸出感測器數據

      if (i != 7) Serial.print(",");

      // 檢查是否超過閥值
      if (uno_car.get(i) < 300) {
        uno_car.leds[i] = HIGH;  // 關閉 LED
      } else {
        uno_car.leds[i] = LOW;  // 開啟LED
      }
    }
    Serial.println();
  }
}
