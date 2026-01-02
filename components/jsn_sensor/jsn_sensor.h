#pragma once

#include "esphome.h"
#include "esphome/components/uart/uart.h"

namespace jsn_sensor {

using namespace esphome;
using namespace esphome::sensor;
using namespace esphome::uart;

class JSNSensor : public PollingComponent, public Sensor {
 public:
  explicit JSNSensor(UARTComponent *parent_uart) 
      : PollingComponent(500), uart(parent_uart) {}

  void setup() override {
    buffer_pos = 0;
  }

  void update() override {
    if (!uart) return;

    while (uart->available()) {
      uint8_t c = uart->read();
      buffer[buffer_pos++] = c;

      if (buffer_pos >= 4) {
        // JSN-SR04T UART 回傳兩個 bytes 距離 (mm)
        uint16_t dist = (buffer[0] << 8) | buffer[1];
        float d = dist / 10.0f;  // 轉成 cm
        ESP_LOGD("jsn_sensor", "Distance raw=%u cm=%.1f", dist, d);
        this->publish_state(d);
        buffer_pos = 0;
      }
    }
  }

 protected:
  uart::UARTComponent *uart;
  uint8_t buffer[4]{0};
  uint8_t buffer_pos{0};
};

}  // namespace jsn_sensor
