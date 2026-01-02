import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, uart

CONF_UART_ID = "uart_id"

jsn_ns = cg.esphome_ns.namespace("jsn_sensor")
JSNSensor = jsn_ns.class_("JSNSensor", cg.PollingComponent, sensor.Sensor)

CONFIG_SCHEMA = cv.Schema({
    cv.Required(CONF_UART_ID): cv.use_id(uart.UARTComponent),
    cv.Required("id"): cv.declare_id(JSNSensor),
    cv.Optional("name"): cv.string,
    cv.Optional("unit_of_measurement"): cv.string,
    cv.Optional("icon"): cv.string,
    cv.Optional("device_class"): cv.string,
    cv.Optional("state_class"): cv.string,
    cv.Optional("accuracy_decimals"): cv.positive_int,
}).extend(sensor.SENSOR_SCHEMA)  # 保留 sensor 標準選項

async def to_code(config):
    uart_var = await cg.get_variable(config[CONF_UART_ID])
    var = cg.new_Pvariable(config["id"], uart_var)
    await sensor.register_sensor(var, config)
