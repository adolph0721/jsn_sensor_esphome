#pragma once

#include "esphome.h"

class JSNSensor : public PollingComponent, public Sensor {
 public:
  JSNSensor(UARTComponent *parent) : PollingComponent(500), uart(parent) {}

  void setup() override {
    buffer_pos = 0;
  }

  void update() override {
    while (uart->available()) {
      uint8_t c = uart->read();
      if (buffer_pos < 4) {
        buffer[buffer_pos++] = c;
      }
      if (buffer_pos == 4) {
        // JSN-SR04T UART V3.3 回傳格式：2 Bytes 高低位 + 2 Bytes checksum/保留
        uint16_t distance = (uint16_t(buffer[0]) << 8) | buffer[1];
        float d_cm = float(distance);
        if (d_cm >= 2.0 && d_cm <= 600.0) {
          publish_state(d_cm);
        } else {
          publish_state(NAN);
        }

        // DEBUG log
        ESP_LOGD("jsn_sensor", "Distance: %.1f cm", d_cm);

        buffer_pos = 0;
      }
    }
  }

 private:
  UARTComponent *uart;
  uint8_t buffer[4];
  uint8_t buffer_pos;
};
