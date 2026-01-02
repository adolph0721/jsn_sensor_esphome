#pragma once

#include "esphome.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/uart/uart.h"
#include "esphome/core/component.h"

namespace jsn_sensor {

class JSNSensor : public PollingComponent {
 public:
  JSNSensor(esphome::uart::UARTDevice *parent, esphome::sensor::Sensor *sensor)
      : PollingComponent(500), uart_(parent), sensor_(sensor) {}

  void setup() override {
    buffer_pos_ = 0;
  }

  void update() override {
    uint8_t c;
    while (uart_->read_byte(&c)) {  // 必須傳入指標
      buffer_[buffer_pos_++] = c;
      if (buffer_pos_ >= sizeof(buffer_)) buffer_pos_ = 0;
    }

    float d = parse_distance();
    if (!isnan(d)) {
      ESP_LOGD("jsn_sensor", "Distance: %.1f cm", d);
      sensor_->publish_state(d);
    }
  }

 protected:
  esphome::uart::UARTDevice *uart_;
  esphome::sensor::Sensor *sensor_;
  uint8_t buffer_[32];
  int buffer_pos_;

  float parse_distance() {
    // 解析 JSN-SR04T UART 資料，假設前兩個 byte 為距離 mm
    if (buffer_pos_ >= 2) {
      int dist_mm = (buffer_[0] << 8) | buffer_[1];
      return dist_mm / 10.0f;  // cm
    }
    return NAN;
  }
};

}  // namespace jsn_sensor
