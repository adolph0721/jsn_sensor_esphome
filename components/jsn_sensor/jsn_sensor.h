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

      // JSN-SR04T frame: FF H L SUM
      if (data[0] != 0xFF) {
        ESP_LOGD("jsn", "Frame error: %02X", data[0]);
        continue;
      }

      uint8_t checksum = data[0] + data[1] + data[2];
      if (checksum != data[3]) {
        ESP_LOGD("jsn", "Checksum error");
        continue;
      }

      int distance_mm = (data[1] << 8) | data[2];
      float distance_cm = distance_mm / 10.0f;

      ESP_LOGD("jsn", "Distance: %.1f cm", distance_cm);

      // ⭐ 關鍵：真的送出數值
      publish_state(distance_cm);
    }
  }
};
