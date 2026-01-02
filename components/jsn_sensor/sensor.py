import esphome.config_validation as cv
from esphome.components import sensor, uart
from esphome.cpp_generator import Pvariable, add, cg

CONF_UART_ID = "uart_id"

JSNSensor = sensor.sensor_ns.class_("JSNSensor", cg.PollingComponent, sensor.Sensor)

PLATFORM_SCHEMA = cv.PLATFORM_SCHEMA.extend({
    cv.Required(CONF_UART_ID): cv.use_id(uart.UARTComponent),
}).extend(sensor.SENSOR_SCHEMA)

async def to_code(config):
    uart_var = await cg.get_variable(config[CONF_UART_ID])
    var = Pvariable(config[CONF_ID], uart_var)
    await sensor.register_sensor(var, config)
