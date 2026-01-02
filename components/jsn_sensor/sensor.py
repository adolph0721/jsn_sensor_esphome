import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, uart
from esphome.const import CONF_ID, CONF_NAME, CONF_UNIT_OF_MEASUREMENT, CONF_ACCURACY_DECIMALS

# C++ 類別命名空間
jsn_ns = cg.esphome_ns.namespace("jsn_sensor")
JSNSensor = jsn_ns.class_("JSNSensor", sensor.Sensor, cg.Component)

CONFIG_SCHEMA = cv.Schema({
    cv.Required(CONF_ID): cv.declare_id(JSNSensor),
    cv.Required("uart_id"): cv.use_id(uart.UARTComponent),
    cv.Optional(CONF_NAME): cv.string,
    cv.Optional(CONF_UNIT_OF_MEASUREMENT): cv.string,
    cv.Optional(CONF_ACCURACY_DECIMALS, default=1): cv.int_,
}).extend(sensor.SENSOR_SCHEMA)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID], config["uart_id"])
    await cg.register_component(var, config)
    await sensor.register_sensor(var, config)
