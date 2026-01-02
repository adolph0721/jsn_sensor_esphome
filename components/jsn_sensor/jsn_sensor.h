#pragma once

#include "esphome.h"
#include "esphome/components/uart/uart.h"
#include "esphome/components/sensor/sensor.h"

namespace jsn_sensor {

class JSNSensor : public PollingComponent {
 public:
  JSNSensor(uart::UARTDevice *parent, sensor::Sensor *sensor)
      : PollingComponent(500), uart_(parent), sensor_(sensor), buffer_pos_(0) {}

  void setup() override { buffer_pos_ = 0; }

  void update() override {
    uint8_t c;
    while (uart_->read_byte(&c)) {
      if (c == 0x59) {  // Header byte
        buffer_[0] = c;
        buffer_pos_ = 1;
      } else if (buffer_pos_ > 0) {
        buffer_[buffer_pos_++] = c;
        if (buffer_pos_ == 9) {
          uint16_t distance = buffer_[2] + (buffer_[3] << 8);
          if (sensor_)
            sensor_->publish_state(distance / 10.0f);  // mm -> cm
          buffer_pos_ = 0;
        }
      }
    }
  }

 protected:
  uart::UARTDevice *uart_;
  sensor::Sensor *sensor_;
  uint8_t buffer_[9];
  uint8_t buffer_pos_;
};

}  // namespace jsn_sensor
