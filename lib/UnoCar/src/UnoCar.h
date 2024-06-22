#ifndef _UnoCar_H
#define _UnoCar_H

#include <Arduino.h>
#include <MK_HC595.h>

#include "HC4067.h"

#define MIXER_PIN (13)
#define PWMA_PIN (11)
#define PWMB_PIN (5)
#define AIN1_PIN (9)
#define AIN2_PIN (10)
#define BIN1_PIN (7)
#define BIN2_PIN (6)

#define SIG (A0)
#define LEDS_CK (A1)
#define LEDS_LATCH (A2)
#define LEDS_DATA (A3)

class UnoCar {
 public:
  MK_HC595<1> leds;
  HC4067 mux;

  UnoCar(uint8_t sensor_count = 8)
      : leds(LEDS_DATA, LEDS_LATCH, LEDS_CK),
        mux(2, 3, 4, 12, SIG),
        _sensor_count(min(16, sensor_count)) {}

  void init() {
    leds.setAll(HIGH);  // Turn off all LEDs
  }

  bool update_mux(uint16_t interval = 0) {
    if (millis() - _last_update_mux < interval) {
      return false;
    }

    for (uint8_t i = 0; i < _sensor_count; i++) {
      mux.setChannel(i);
      _mux_values[i] = mux.read();
      delayMicroseconds(50);
    }

    mux.disable();
    _last_update_mux = millis();
    return true;
  }

  uint8_t get(uint8_t index) {
    return _mux_values[min(16, index)];
  }

  uint8_t isTouch() {
    return digitalRead(MIXER_PIN);
  }

 private:
  uint8_t _sensor_count;
  int _mux_values[16];
  uint16_t _last_update_mux;
};

#endif /* _UnoCar_H */
