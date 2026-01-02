#pragma once
#include "esphome.h"

using namespace esphome;

class JSNSensor : public Sensor, public Component, public UARTDevice {
 public:
  JSNSensor(UARTComponent *parent) : UARTDevice(parent) {}

  void loop() override {
    while (available() >= 4) {
      uint8_t data[4];
      read_array(data, 4);

      if (data[0] != 0xFF) continue;
      uint8_t checksum = data[0] + data[1] + data[2];
      if (checksum != data[3]) continue;

      int distance_mm = (data[1] << 8) | data[2];
      float distance_cm = distance_mm / 10.0f;

      publish_state(distance_cm);  // ⭐ 關鍵
    }
  }
};
