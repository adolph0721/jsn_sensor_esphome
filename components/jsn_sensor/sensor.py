import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, uart
from esphome.const import CONF_ID, CONF_NAME, CONF_UNIT_OF_MEASUREMENT, CONF_ICON, CONF_ACCURACY_DECIMALS

CONF_UART_ID = "uart_id"

jsn_ns = cg.esphome_ns.namespace("jsn_sensor")
JSNSensor = jsn_ns.class_("JSNSensor", cg.Component)

# 新版 schema 寫法，ESPHome 2025
PLATFORM_SCHEMA = cv.Schema({
    cv.Required(CONF_ID): cv.declare_id(JSNSensor),
    cv.Required(CONF_UART_ID): cv.use_id(uart.UARTDevice),
    cv.Optional(CONF_NAME): cv.string,
    cv.Optional(CONF_UNIT_OF_MEASUREMENT): cv.string,
    cv.Optional(CONF_ICON): cv.icon,
    cv.Optional(CONF_ACCURACY_DECIMALS, default=1): cv.int_,
})

async def to_code(config):
    uart_dev = await cg.get_variable(config[CONF_UART_ID])
    var = cg.new_Pvariable(config[CONF_ID], uart_dev)

    # 註冊 Component
    await cg.register_component(var, config)

    # 註冊 sensor
    sens = cg.new_Pvariable(config.get(CONF_ID))
    if CONF_NAME in config:
        sens.set_name(config[CONF_NAME])
    if CONF_UNIT_OF_MEASUREMENT in config:
        sens.set_unit_of_measurement(config[CONF_UNIT_OF_MEASUREMENT])
    if CONF_ACCURACY_DECIMALS in config:
        sens.set_accuracy_decimals(config[CONF_ACCURACY_DECIMALS])
    await sensor.register_sensor(var, sens, config)
