import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, uart

CONF_UART_ID = "uart_id"

# C++ class 與 namespace
jsn_ns = cg.esphome_ns.namespace("jsn_sensor")
JSNSensor = jsn_ns.class_("JSNSensor", cg.PollingComponent, sensor.Sensor)

# 新版 ESPHome schema
CONFIG_SCHEMA = cv.Schema({
    cv.Required(CONF_UART_ID): cv.use_id(uart.UARTComponent),
    cv.Required(sensor.CONF_ID): cv.declare_id(JSNSensor),
})

async def to_code(config):
    uart_var = await cg.get_variable(config[CONF_UART_ID])
    var = cg.new_Pvariable(config[sensor.CONF_ID], uart_var)
    await sensor.register_sensor(var, config)
