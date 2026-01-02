#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/uart/uart.h"
#include "esphome/core/log.h"

namespace esphome {
namespace jsn_sensor {

static const char *const TAG = "jsn_sensor";

class JSNSensor : public sensor::Sensor,
                  public Component,
                  public uart::UARTDevice {
 public:
  explicit JSNSensor(uart::UARTComponent *parent)
      : uart::UARTDevice(parent) {}

  void loop() override {
    while (this->available() >= 4) {
      uint8_t data[4];
      this->read_array(data, 4);

      // JSN-SR04T frame: FF H L SUM
      if (data[0] != 0xFF) {
        ESP_LOGD(TAG, "Frame header error: %02X", data[0]);
        continue;
      }

      uint8_t checksum = data[0] + data[1] + data[2];
      if (checksum != data[3]) {
        ESP_LOGD(TAG, "Checksum error");
        continue;
      }

      int distance_mm = (data[1] << 8) | data[2];
      float distance_cm = distance_mm / 10.0f;

      ESP_LOGD(TAG, "Distance %.1f cm", distance_cm);

      // ⭐ 關鍵：真正送出數值
      this->publish_state(distance_cm);
    }
  }
};

}  // namespace jsn_sensor
}  // namespace esphome
