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

#define RULE_SPEED(speed) (constrain(abs(speed), 0, 255))

#ifndef DEFAULT_SPEED
  #define DEFAULT_SPEED 255
#endif

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

  inline int get(uint8_t index) {
    mux.setChannel(index);
    return mux.read();
  }

  inline uint8_t getChannel(uint8_t index) {
    return mux.getChannel();
  }

  inline uint8_t isTouch() {
    return !digitalRead(MIXER_PIN);
  }

  void drive(int speed) {
    driveA(speed);
    driveB(speed);
  }

  void drive(int speedA, int speedB) {
    _driveAB(speedA, speedB);
  }

  inline void driveA(int speed = DEFAULT_SPEED) {
    _drive(AIN1_PIN, AIN2_PIN, PWMA_PIN, speed);
  }

  inline void driveB(int speed = DEFAULT_SPEED) {
    _drive(BIN2_PIN, BIN1_PIN, PWMB_PIN, speed);
  }

  void brakeA() {
    driveA(0);
  }
  inline void stopA() {
    brakeA();
  }

  void brakeB() {
    driveB(0);
  }
  inline void stopB() {
    brakeB();
  }

  void brake() {
    stopA();
    stopB();
  }
  inline void stop() {
    brake();
  }

  void standby() {
    digitalWrite(STBY_PIN, LOW);
  }

  void forward(int speed = DEFAULT_SPEED) {
    speed = RULE_SPEED(speed);

    drive(speed);
  }
  void forward(int speedA, int speedB) {
    _driveAB(RULE_SPEED(speedA), RULE_SPEED(speedB));
  }

  void back(int speed = DEFAULT_SPEED) {
    speed = -RULE_SPEED(speed);

    drive(speed);
  }
  void back(int speedA, int speedB) {
    _driveAB(-RULE_SPEED(speedA), -RULE_SPEED(speedB));
  }

  void left(int speed = DEFAULT_SPEED) {
    speed = RULE_SPEED(speed);

    _driveAB(-speed, speed);
  }

  void right(int speed = DEFAULT_SPEED) {
    speed = RULE_SPEED(speed);

    _driveAB(speed, -speed);
  }

 private:
  uint8_t _sensor_count;
  int _values[16];
  uint16_t _last_update_mux;

  void _drive(uint8_t in1, uint8_t in2, uint8_t speed_pin, int speed) {
    digitalWrite(STBY_PIN, HIGH);

    digitalWrite(in1, speed > 0);
    digitalWrite(in2, speed < 0);
    analogWrite(speed_pin, abs(speed));
  }

  inline void _driveAB(int speedA, int speedB) {
    driveA(speedA);
    driveB(speedB);
  }
};

#endif /* _UnoCar_H */
