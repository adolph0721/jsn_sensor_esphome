#pragma once

#include "esphome/components/sensor/sensor.h"
#include "esphome/components/uart/uart.h"
#include "esphome/core/component.h"

namespace esphome {
namespace jsn_sensor {

class JSNSensor : public Component {
 public:
  explicit JSNSensor(uart::UARTComponent *parent, sensor::Sensor *sensor)
      : uart_(parent), sensor_(sensor) {}

  void setup() override {
    buffer_pos_ = 0;
  }

  void loop() override {
    uint8_t c;
    while (uart_->read_byte(&c)) {
      if (buffer_pos_ == 0 && c != 0xFF) continue; // header
      if (buffer_pos_ == 0) {
        buffer_[0] = c;
        buffer_pos_ = 1;
      } else if (buffer_pos_ > 0) {
        buffer_[buffer_pos_++] = c;
      }

      if (buffer_pos_ == 4) { // 假設 4 bytes JSN-SR04T
        int distance = (buffer_[2] << 8) | buffer_[3];
        sensor_->publish_state(distance / 10.0f); // mm -> cm
        buffer_pos_ = 0;
      }
    }
  }

 protected:
  uart::UARTComponent *uart_;
  sensor::Sensor *sensor_;
  uint8_t buffer_[4];
  uint8_t buffer_pos_;
};

}  // namespace jsn_sensor
}  // namespace esphome
