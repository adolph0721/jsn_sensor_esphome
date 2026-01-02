#pragma once

#include "esphome.h"
#include "esphome/components/uart/uart.h"

namespace jsn_sensor {

class JSNSensor : public PollingComponent, public Sensor {
 public:
  // 使用 UARTComponent 而不是 UARTDevice
  JSNSensor(esphome::uart::UARTComponent *parent) 
    : PollingComponent(500), uart_(parent) {}

  void setup() override {
    buffer_pos_ = 0;
  }

  void update() override {
    while (uart_->available()) {
      uint8_t c = 0;
      if (!uart_->read_byte(&c)) break;  // 使用 read_byte(uint8_t*)
      buffer_[buffer_pos_] = c;
      buffer_pos_++;

      if (buffer_pos_ >= 4) {  // JSN-SR04T 每筆資料4 bytes
        // 計算距離 (mm)
        uint16_t val = (uint16_t(buffer_[2]) << 8) | buffer_[3];
        float d = val / 10.0;  // cm
        publish_state(d);
        buffer_pos_ = 0;
      }
    }
  }

 protected:
  esphome::uart::UARTComponent *uart_;
  uint8_t buffer_[4];
  uint8_t buffer_pos_;
};

}  // namespace jsn_sensor
