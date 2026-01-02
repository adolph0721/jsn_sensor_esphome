#pragma once

#include "esphome.h"
#include "esphome/components/uart/uart.h"

namespace jsn_sensor {

class JSNSensor : public PollingComponent {
 public:
  explicit JSNSensor(uart::UARTDevice *parent) : PollingComponent(500), uart_(parent) {}

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
          publish_state(distance / 10.0f);  // mm → cm
          buffer_pos_ = 0;
        }
      }
    }
  }

  void set_name(std::string name) { this->name_ = name; }
  void set_unit_of_measurement(std::string unit) { this->unit_ = unit; }
  void set_accuracy_decimals(int decimals) { this->decimals_ = decimals; }

 protected:
  uart::UARTDevice *uart_;
  uint8_t buffer_[9];
  uint8_t buffer_pos_;
  std::string name_;
  std::string unit_;
  int decimals_;
};

}  // namespace jsn_sensor
