#include <Arduino.h>
#include <UnoCar.h>

#define SENSOR_COUNT 8

UnoCar uno_car(SENSOR_COUNT);

void setup() {
  uno_car.init();  // 初始化 UnoCar
}

void loop() {
  // 全部燈閃爍
  // for (int i = 0; i < 3; i++) {
  //   uno_car.leds.setAll(HIGH);  // 關閉所有燈
  //   delay(200);
  //   uno_car.leds.setAll(LOW);  // 開啟所有燈
  //   delay(200);
  // }

  // 跑馬燈
  // for (int i = 0; i < 8; i++) {
  //   uno_car.leds[0] = 1;  // 設定第一個LED為暗
  //   delay(400);
  //   uno_car.leds.move(1);  // 設定LED往下移動
  // }

  // 第四個LED亮滅
  // uno_car.leds[3] = HIGH;  // 設定第四個LED為暗
  // delay(400);
  // uno_car.leds[3] = HIGH;  // 設定第五個LED為暗
  // delay(400);

  // 全部燈亮滅
  // uno_car.leds.toggleAll();  // 切換所有LED狀態
  // delay(400);
  // uno_car.leds.toggleAll();  // 切換所有LED狀態
  // delay(400);

  uno_car.leds.setAll(HIGH);  // 關閉所有燈
  delay(500);
  for (int i = 0; i < 8; i++) {
    uno_car.leds[i] = LOW;  // 設定第 (i + 1) 個LED為開
    delay(200);
    uno_car.leds[i] = HIGH;  // 設定第 (i + 1) 個LED為關
    delay(200);
  }
  delay(500);
}
