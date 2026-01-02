import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart

CONF_UART_ID = "uart_id"

jsn_ns = cg.esphome_ns.namespace("jsn_sensor")
JSNSensor = jsn_ns.class_("JSNSensor", cg.PollingComponent)

CONFIG_SCHEMA = cv.Schema({
    cv.Required(CONF_UART_ID): cv.use_id(uart.UARTComponent),
    cv.Required("id"): cv.declare_id(JSNSensor),
    cv.Optional("name"): cv.string,
    cv.Optional("unit_of_measurement"): cv.string,
    cv.Optional("icon"): cv.string,
    cv.Optional("device_class"): cv.string,
    cv.Optional("state_class"): cv.string,
    cv.Optional("accuracy_decimals"): cv.positive_int,
})

async def to_code(config):
    uart_var = await cg.get_variable(config[CONF_UART_ID])
    var = cg.new_Pvariable(config["id"], uart_var)
    cg.add(var.set_name(config.get("name", "")))
    cg.add(var.set_unit_of_measurement(config.get("unit_of_measurement", "")))
    cg.add(var.set_accuracy_decimals(config.get("accuracy_decimals", 1)))
