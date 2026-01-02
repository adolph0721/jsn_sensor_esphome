import esphome.config_validation as cv
from esphome.components import sensor, uart
from esphome import pins
import esphome.codegen as cg

CONF_UART_ID = "uart_id"

jsn_sensor_ns = cg.esphome_ns.namespace("jsn_sensor")
JSNSensor = jsn_sensor_ns.class_("JSNSensor", cg.PollingComponent, sensor.Sensor)

PLATFORM_SCHEMA = sensor.SENSOR_PLATFORM_SCHEMA.extend({
    cv.Required(CONF_UART_ID): cv.use_id(uart.UARTComponent),
})

async def to_code(config):
    uart_var = await cg.get_variable(config[CONF_UART_ID])
    var = cg.new_Pvariable(config[CONF_ID], uart_var)
    await sensor.register_sensor(var, config)
