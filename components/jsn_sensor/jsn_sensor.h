#pragma once

#include "esphome.h"
#include "esphome/components/uart/uart.h"

namespace jsn_sensor {

class JSNSensor : public sensor::Sensor, public PollingComponent {
 public:
  JSNSensor(uart::UARTDevice *parent) : PollingComponent(500), uart_(parent) {}

  void setup() override {
    buffer_pos_ = 0;
  }

  void update() override {
    // 讀取 UART bytes
    while (uart_->available()) {
      uint8_t c = uart_->read_byte();
      buffer_[buffer_pos_++] = c;
      if (buffer_pos_ >= sizeof(buffer_)) buffer_pos_ = 0;
    }

    // 解析距離
    float d = parse_distance();
    if (!isnan(d)) {
      ESP_LOGD("jsn_sensor", "Distance: %.1f cm", d);
      publish_state(d);
    }
  }

 protected:
  uart::UARTDevice *uart_;
  uint8_t buffer_[32];
  int buffer_pos_;

  float parse_distance() {
    // 這裡要放你的 JSN-SR04T UART 資料解析邏輯
    // 範例: 假設 buffer[0] = 高位，buffer[1] = 低位
    if (buffer_pos_ >= 2) {
      float dist = buffer_[0] << 8 | buffer_[1];
      return dist / 10.0;  // 如果 sensor 傳回 mm，轉成 cm
    }
    return NAN;
  }
};

}  // namespace jsn_sensor
