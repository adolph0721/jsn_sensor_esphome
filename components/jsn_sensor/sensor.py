import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, uart

# 定義 namespace 與 C++ class
jsn_sensor_ns = cg.esphome_ns.namespace("jsn_sensor")
JSNSensor = jsn_sensor_ns.class_("JSNSensor", cg.PollingComponent, sensor.Sensor)

# 新版 schema 定義，不依賴 SENSOR_SCHEMA
CONFIG_SCHEMA = cv.Schema({
    cv.Required("uart_id"): cv.use_id(uart.UARTComponent),
    cv.Required("name"): cv.string,
    cv.Optional("id"): cv.declare_id(JSNSensor),
}).extend({})  # 不再 extend sensor.SENSOR_SCHEMA

async def to_code(config):
    uart_ = await cg.get_variable(config["uart_id"])
    name = config["name"]
    # 如果有 id 就用，沒有就自己生成
    if "id" in config:
        var = cg.new_Pvariable(config["id"], uart_)
    else:
        var = cg.new_Pvariable(name, uart_)
    await sensor.register_sensor(var, config)
