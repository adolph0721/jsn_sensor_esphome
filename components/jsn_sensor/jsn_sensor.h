#pragma once
#include "esphome.h"
#include "esphome/components/uart/uart.h"

namespace jsn_sensor {

class JSNSensor : public PollingComponent {
 public:
  JSNSensor(esphome::uart::UARTComponent *parent) 
      : PollingComponent(500), uart_(parent) {}

  void setup() override {
    ESP_LOGD("jsn_sensor", "JSN Sensor setup");
    buffer_pos_ = 0;
  }

  void update() override {
    uint8_t byte;
    while (uart_->read_byte(&byte)) {
      if (byte == '\n') {
        float d = atof(buffer_) / 10.0;
        ESP_LOGD("jsn_sensor", "Water distance: %.1f cm", d);
        buffer_pos_ = 0;
      } else if (buffer_pos_ < sizeof(buffer_) - 1) {
        buffer_[buffer_pos_++] = byte;
        buffer_[buffer_pos_] = 0;
      }
    }
  }

 protected:
  esphome::uart::UARTComponent *uart_;
  char buffer_[32];
  size_t buffer_pos_;
};

}  // namespace jsn_sensor
