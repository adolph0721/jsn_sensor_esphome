#pragma once
#include "esphome.h"
#include "esphome/components/uart/uart.h"
#include "esphome/components/sensor/sensor.h"

namespace jsn_sensor {

class JSNSensor : public esphome::Component {
 public:
  JSNSensor(esphome::uart::UARTDevice *parent, esphome::sensor::Sensor *sensor)
      : uart_(parent), sensor_(sensor) {}

  void setup() override {
    buffer_pos_ = 0;
  }

  void loop() override {
    uint8_t c;
    while (uart_->read_byte(&c)) {  // 讀取 UART
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
    if (buffer_pos_ >= 2) {
      int dist_mm = (buffer_[0] << 8) | buffer_[1];
      return dist_mm / 10.0f;  // cm
    }
    return NAN;
  }
};

}  // namespace jsn_sensor
