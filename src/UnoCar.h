#ifndef _UnoCar_H
#define _UnoCar_H

#include <Arduino.h>

#include "HC4067.h"
#include "MKHC595.h"

#define MIXER_PIN 13
#define PWMA_PIN 11
#define PWMB_PIN 5
#define AIN1_PIN 9
#define AIN2_PIN 10
#define BIN1_PIN 7
#define BIN2_PIN 6
#define STBY_PIN 8

#define SIG A0
#define S0 2
#define S1 3
#define S2 4
#define S3 12

#define LEDS_CK A1
#define LEDS_LATCH A2
#define LEDS_DATA A3

class UnoCar {
 public:
  MKHC595<1> leds;
  HC4067 mux;

  UnoCar(uint8_t sensor_count = 8)
      : leds(LEDS_DATA, LEDS_LATCH, LEDS_CK),
        mux(S0, S1, S2, S3, SIG),
        _sensor_count(min(16, sensor_count)) {}

  /**
   * Initializes the UnoCar by turning off all LEDs, setting up mixer pin,
   * setting analog reference, and configuring various pins.
   *
   * @return None
   */
  void init() {
    leds.setAll(HIGH);  // Turn off all LEDs

    pinMode(MIXER_PIN, INPUT);
    digitalWrite(MIXER_PIN, HIGH);

    analogReference(EXTERNAL);

    pinMode(PWMA_PIN, OUTPUT);
    pinMode(PWMB_PIN, OUTPUT);
    pinMode(AIN1_PIN, OUTPUT);
    pinMode(AIN2_PIN, OUTPUT);
    pinMode(BIN1_PIN, OUTPUT);
    pinMode(BIN2_PIN, OUTPUT);
    pinMode(STBY_PIN, OUTPUT);
  }

  /**
   * Updates the multiplexer with the values of each channel.
   *
   * @param interval The time interval in milliseconds between updates. If not
   * specified, the function will update the mux immediately.
   *
   * @return `true` if the mux was updated, `false` if the update was skipped
   * because the interval has not passed yet.
   */
  bool update_mux(uint16_t interval = 0) {
    if (millis() - _last_update_mux < interval) {
      return false;
    }

    for (uint8_t i = 0; i < _sensor_count; i++) {
      mux.setChannel(i);
      _values[i] = mux.read();
    }

    mux.disable();
    _last_update_mux = millis();
    return true;
  }

  /**
   * Returns the value at the specified index in the `_values` array.
   * _values contains MUX read values.
   *
   * @param index the index of the value to retrieve
   *
   * @return the value at the specified index, or 0 if the index is greater than
   * 15
   */
  inline uint8_t get(uint8_t index) {
    return _values[min(16, index)];
  }

  inline uint8_t isTouch() {
    return !digitalRead(MIXER_PIN);
  }

  inline void driveA(int speed) {
    _drive(AIN1_PIN, AIN2_PIN, PWMA_PIN, speed);
  }

  inline void driveB(int speed) {
    _drive(BIN1_PIN, BIN2_PIN, PWMB_PIN, speed);
  }

  void brakeA() {
    driveA(0);
  }
  inline void stopA() {
    brakeA();
  }

  void brakeB() {
    driveA(0);
  }
  inline void stopB() {
    brakeB();
  }

  void standby() {
    digitalWrite(STBY_PIN, LOW);
  }

  void forward(uint8_t speed) {
    driveA(speed);
    driveB(speed);
  }
  void forward(uint8_t speedA = 255, uint8_t speedB = 255) {
    driveA(speedA);
    driveB(speedB);
  }

  void back(uint8_t speed) {
    driveA(-speed);
    driveB(-speed);
  }
  void back(uint8_t speedA = 255, uint8_t speedB = 255) {
    driveA(-speedA);
    driveB(-speedB);
  }

  void left(uint8_t speed) {
    uint8_t temp = speed / 2;
    driveA(-temp);
    driveB(temp);
  }

  void right(uint8_t speed) {
    uint8_t temp = speed / 2;
    driveA(temp);
    driveB(-temp);
  }

 private:
  uint8_t _sensor_count;
  int _values[16];
  uint16_t _last_update_mux;

  void _drive(uint8_t in1, uint8_t in2, uint8_t speed_pin, int speed) {
    digitalWrite(STBY_PIN, HIGH);

    digitalWrite(in1, speed > 0);
    digitalWrite(in2, speed < 0);
    analogWrite(speed_pin, speed);
  }
};

#endif /* _UnoCar_H */
