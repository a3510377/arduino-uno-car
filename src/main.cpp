#include <Arduino.h>
#include <UnoCar.h>

#define LED_START_FLASHES (0)

UnoCar uno_car(8);

void setup() {
  Serial.begin(9600);

  uno_car.init();
  pinMode(13, INPUT);
  digitalWrite(13, HIGH);
}

void loop() {
  uno_car.update_mux(50);

  for (int i = 0; i < 8; i++) {
    Serial.print(uno_car.get(i));
    Serial.print(",");
  }

  Serial.println();
}
