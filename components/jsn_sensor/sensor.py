import esphome.config_validation as cv
from esphome.components import sensor, uart
from esphome.const import CONF_ID
import esphome.codegen as cg

CONF_UART_ID = "uart_id"

jsn_ns = cg.esphome_ns.namespace("jsn_sensor")
JSNSensor = jsn_ns.class_("JSNSensor", cg.Component)

PLATFORM_SCHEMA = sensor.SENSOR_PLATFORM_SCHEMA.extend({
    cv.Required(CONF_UART_ID): cv.use_id(uart.UARTDevice),
})

async def to_code(config):
    uart_dev = await cg.get_variable(config[CONF_UART_ID])
    sens = sensor.new_sensor(config)
    var = cg.new_Pvariable(config[CONF_ID], uart_dev, sens)
    await cg.register_component(var, config)
    await sensor.register_sensor(var, sens, config)
