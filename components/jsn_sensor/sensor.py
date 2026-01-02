import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, uart

CONF_UART_ID = "uart_id"

jsn_ns = cg.esphome_ns.namespace("jsn_sensor")
JSNSensor = jsn_ns.class_("JSNSensor", cg.PollingComponent, sensor.Sensor)

CONFIG_SCHEMA = cv.Schema({
    cv.Required(CONF_UART_ID): cv.use_id(uart.UARTComponent),
    cv.Required(sensor.CONF_ID): cv.declare_id(JSNSensor),
    cv.Optional(sensor.CONF_NAME): cv.string,
    cv.Optional(sensor.CONF_UNIT_OF_MEASUREMENT): cv.string,
    cv.Optional(sensor.CONF_ICON): cv.icon,
    cv.Optional(sensor.CONF_DEVICE_CLASS): sensor.DEVICE_CLASSES_SCHEMA,
    cv.Optional(sensor.CONF_STATE_CLASS): sensor.STATE_CLASSES_SCHEMA,
    cv.Optional(sensor.CONF_ACCURACY_DECIMALS): cv.positive_int,
}).extend(sensor.SENSOR_SCHEMA)  # 把 sensor 基本選項加入 schema

async def to_code(config):
    uart_var = await cg.get_variable(config[CONF_UART_ID])
    var = cg.new_Pvariable(config[sensor.CONF_ID], uart_var)
    await sensor.register_sensor(var, config)
