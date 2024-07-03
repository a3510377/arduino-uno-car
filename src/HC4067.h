#ifndef _HC4067_H
#define _HC4067_H

#include <Arduino.h>

class HC4067 {
 public:
  // Constructor: initializes the multiplexer with the given control pins and
  // signal pin.
  HC4067(uint8_t s0, uint8_t s1, uint8_t s2, uint8_t s3, uint8_t sig = A0,
         uint8_t e = 255) {
    _pins[0] = s0;
    _pins[1] = s1;
    _pins[2] = s2;
    _pins[3] = s3;
    _enablePin = e;
    _sig = sig;

    // Set the control pins as output
    for (uint8_t i = 0; i < 4; i++) {
      pinMode(_pins[i], OUTPUT);
      digitalWrite(_pins[i], HIGH);
    }

    // If an enable pin is provided, set it as output and disable by default
    if (_enablePin != 255) {
      pinMode(_enablePin, OUTPUT);
      // Default to disabled
      digitalWrite(_enablePin, HIGH);
    }
  }

  // Enable the multiplexer
  void enable() {
    if (_enablePin != 255) {
      digitalWrite(_enablePin, LOW);
    }
  }

  // Check if the multiplexer is enabled
  bool isEnable() {
    return _enablePin != 255 ? digitalRead(_enablePin) == LOW : true;
  }

  // Disable the multiplexer
  void disable() {
    if (_enablePin != 255) {
      digitalWrite(_enablePin, LOW);
    }
  }

  // Set the channel of the multiplexer, optionally disable it first to avoid
  // ghost channels
  bool setChannel(uint8_t channel, bool disable = true) {
    if (channel > 15) {
      // Invalid channel
      return false;
    }

    if (disable) {
      // prevent ghost channels.
      this->disable();
    }

    uint8_t _change = channel ^ _channel;
    for (uint8_t i = 0; i < 4; i++) {
      // only write changes(for fast switching)
      if (_change & 1) {
        digitalWrite(_pins[i], (channel >> i) & 1);
      }
      _change >>= 1;
    }

    enable();
    _channel = channel;
    return true;
  }

  // Read an analog value from the signal pin
  inline int read() {
    return analogRead(_sig);
  }

  // Read a digital value from the signal pin
  inline bool boolRead() {
    return digitalRead(_sig);
  }

  // Get the currently selected channel
  inline uint8_t getChannel() {
    return _channel;
  }

  // Move to the next channel
  inline bool next() {
    return setChannel((_channel + 1) % 16);
  }

  // Move to the previous channel
  inline bool prev() {
    return setChannel((_channel + 15) % 16);
  }

 protected:
  uint8_t _pins[4];          // Control pins
  uint8_t _enablePin = 255;  // Enable pin, default is not used
  uint8_t _channel = 0xf;    // Current channel
  uint8_t _sig = A0;         // Signal pin, default is A0
};

#endif /* _HC4067_H */
