#pragma once

#include "esphome.h"
#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/uart/uart.h"

namespace jsn_sensor {

class JSNSensor : public PollingComponent, public esphome::sensor::Sensor {
 public:
  JSNSensor(esphome::uart::UARTDevice *parent) : PollingComponent(500), uart_(parent) {}

  void setup() override {
    buffer_pos_ = 0;
  }

  void update() override {
    uint8_t byte;
    while (uart_->read_byte(&byte)) {
      buffer_[buffer_pos_++] = byte;
      if (buffer_pos_ == 4) {  // JSN-SR04T UART 4 bytes: 0xFF, high, low, checksum
        if (buffer_[0] == 0xFF) {
          int distance = (buffer_[1] << 8) | buffer_[2];  // mm
          publish_state(distance / 10.0f);  // 發佈 cm
        }
        buffer_pos_ = 0;
      }
    }
  }

 protected:
  esphome::uart::UARTDevice *uart_;
  uint8_t buffer_[4];
  uint8_t buffer_pos_;
};

}  // namespace jsn_sensor
