#pragma once

#include "esphome.h"
#include "esphome/components/uart/uart.h"
#include "esphome/components/sensor/sensor.h"  // <-- 必須 include sensor

namespace jsn_sensor {

class JSNSensor : public esphome::sensor::Sensor, public PollingComponent {
 public:
  JSNSensor(esphome::uart::UARTDevice *parent) 
    : PollingComponent(500), uart_(parent) {}

  void setup() override {
    buffer_pos_ = 0;
  }

  void update() override {
    while (uart_->available()) {
      uint8_t c = uart_->read_byte();
      buffer_[buffer_pos_++] = c;
      if (buffer_pos_ >= sizeof(buffer_)) buffer_pos_ = 0;
    }

    float d = parse_distance();
    if (!isnan(d)) {
      ESP_LOGD("jsn_sensor", "Distance: %.1f cm", d);
      publish_state(d);
    }
  }

 protected:
  esphome::uart::UARTDevice *uart_;
  uint8_t buffer_[32];
  int buffer_pos_;

  float parse_distance() {
    // TODO: 這裡放 JSN-SR04T UART 資料解析邏輯
    if (buffer_pos_ >= 2) {
      float dist = buffer_[0] << 8 | buffer_[1];
      return dist / 10.0;  // 假設 sensor 傳回 mm
    }
    return NAN;
  }
};

}  // namespace jsn_sensor
