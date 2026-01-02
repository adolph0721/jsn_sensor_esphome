#pragma once

#include "esphome.h"
#include "esphome/components/uart/uart.h"

namespace jsn_sensor {

class JSNSensor : public PollingComponent {
 public:
  JSNSensor(esphome::uart::UARTDevice *parent)
      : PollingComponent(500), uart_(parent) {}

  void setup() override {
    buffer_pos_ = 0;
  }

  void update() override {
    uint8_t byte;
    while (uart_->read_byte(&byte)) {  // 使用 read_byte(&byte)
      buffer_[buffer_pos_++] = byte;
      if (buffer_pos_ >= sizeof(buffer_)) buffer_pos_ = 0;
    }

    // 假設每次 update 就簡單計算距離示範
    // JSN-SR04T UART 解析邏輯可以依照實際規格修改
    if (buffer_pos_ >= 2) {
      uint16_t distance = (buffer_[0] << 8) | buffer_[1];
      publish_state(distance / 10.0f);  // 例如 mm → cm
      buffer_pos_ = 0;
    }
  }

 protected:
  esphome::uart::UARTDevice *uart_;
  uint8_t buffer_[10];
  size_t buffer_pos_;
};

}  // namespace jsn_sensor
