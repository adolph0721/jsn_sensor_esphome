import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, uart

# 告訴 ESPHome namespace
jsn_sensor_ns = cg.esphome_ns.namespace("jsn_sensor")
JSNSensor = jsn_sensor_ns.class_("JSNSensor", cg.PollingComponent, sensor.Sensor)

# 新版 schema 定義方式
CONFIG_SCHEMA = cv.Schema({
    cv.Required("uart_id"): cv.use_id(uart.UARTComponent),
    cv.GenerateID(): cv.declare_id(JSNSensor),
}).extend(sensor.SENSOR_SCHEMA)

async def to_code(config):
    uart_ = await cg.get_variable(config["uart_id"])
    var = cg.new_Pvariable(config[cv.GenerateID()], uart_)
    await sensor.register_sensor(var, config)
