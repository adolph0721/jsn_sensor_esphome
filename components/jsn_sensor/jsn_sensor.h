#pragma once

#include "esphome.h"
#include "esphome/components/uart/uart.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/core/component.h"

namespace jsn_sensor {

class JSNSensor : public PollingComponent, public esphome::sensor::Sensor {
 public:
  JSNSensor(esphome::uart::UARTDevice *parent) 
      : PollingComponent(500), uart_(parent) {}

  void setup() override {
    buffer_pos_ = 0;
  }

  void update() override {
    uint8_t c;
    while (uart_->read_byte(&c)) {  // <-- 修正這裡
      buffer_[buffer_pos_++] = c;
      if (buffer_pos_ >= sizeof(buffer_)) buffer_pos_ = 0;
    }

    float d = parse_distance();
    if (!isnan(d)) {
      ESP_LOGD("jsn_sensor", "Distance: %.1f cm", d);  // DEBUG log
      publish_state(d);
    }
  }

 protected:
  esphome::uart::UARTDevice *uart_;
  uint8_t buffer_[32];
  int buffer_pos_;

  float parse_distance() {
    // TODO: 解析 JSN-SR04T UART 傳來的兩個 byte
    if (buffer_pos_ >= 2) {
      float dist = (buffer_[0] << 8) | buffer_[1];
      return dist / 10.0;  // 假設 sensor 傳回 mm
    }
    return NAN;
  }
};

}  // namespace jsn_sensor
