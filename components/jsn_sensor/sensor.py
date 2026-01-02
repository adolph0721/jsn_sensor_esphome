import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart, sensor

CONF_UART_ID = "uart_id"
CONF_SENSOR = "sensor"

jsn_ns = cg.esphome_ns.namespace("jsn_sensor")
JSNSensor = jsn_ns.class_("JSNSensor", cg.PollingComponent)

CONFIG_SCHEMA = cv.Schema({
    cv.Required("id"): cv.declare_id(JSNSensor),
    cv.Required(CONF_UART_ID): cv.use_id(uart.UARTComponent),
    cv.Required(CONF_SENSOR): cv.use_id(sensor.Sensor),
})

async def to_code(config):
    uart_var = await cg.get_variable(config[CONF_UART_ID])
    sensor_var = await cg.get_variable(config[CONF_SENSOR])
    var = cg.new_Pvariable(config["id"], uart_var, sensor_var)
