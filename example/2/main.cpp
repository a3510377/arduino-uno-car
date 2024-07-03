#include <Arduino.h>
#include <UnoCar.h>

#define SENSOR_COUNT 8

UnoCar uno_car(SENSOR_COUNT);

void setup() {
  Serial.begin(9600);

  uno_car.init();  // 初始化 UnoCar

  for (int i = 0; i < 3; i++) {
    uno_car.leds.setAll(HIGH);  // 關閉所有燈
    delay(200);
    uno_car.leds.setAll(LOW);  // 開啟所有燈
    delay(200);
  }

  for (int i = 0; i < 8; i++) {
    uno_car.leds[0] = 1;  // 設定第一個LED為暗
    delay(500);
    uno_car.leds.move(1);  // 設定LED往下移動
  }
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

  if (uno_car.isTouch()) {     // 檢查是否觸碰到球塔
    uno_car.leds.setAll(LOW);  // 開啟所有燈
    delay(1000);
    uno_car.leds.setAll(HIGH);  // 關閉所有燈
  }
}
