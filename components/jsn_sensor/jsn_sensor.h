#pragma once

#include "esphome.h"
#include "esphome/components/uart/uart.h"

namespace jsn_sensor {

class JSNSensor : public PollingComponent {
 public:
  JSNSensor(esphome::uart::UARTDevice *parent) : PollingComponent(500), uart_(parent) {}

  void setup() override {
    ESP_LOGD("jsn_sensor", "JSN Sensor setup");
    buffer_pos_ = 0;
  }

  void update() override {
    uint8_t data;
    while (uart_->read_byte(&data)) {
      // 簡單解析 UART 回傳的 2 bytes 距離數據
      buffer_[buffer_pos_++] = data;
      if (buffer_pos_ >= sizeof(buffer_)) buffer_pos_ = 0;

      if (buffer_pos_ == 2) {
        float d = float((buffer_[0] << 8) | buffer_[1]) / 10.0;
        ESP_LOGD("jsn_sensor", "Distance: %.1f cm", d);
        if (distance_sensor_) distance_sensor_->publish_state(d);
      }
    }
  }

  void set_sensor(esphome::sensor::Sensor *sensor) { distance_sensor_ = sensor; }

 protected:
  esphome::uart::UARTDevice *uart_;
  esphome::sensor::Sensor *distance_sensor_{nullptr};
  uint8_t buffer_[2];
  uint8_t buffer_pos_;
};

}  // namespace jsn_sensor
