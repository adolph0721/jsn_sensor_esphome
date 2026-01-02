import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, uart
from esphome import pins

jsn_sensor_ns = cg.esphome_ns.namespace("jsn_sensor")
JSNSensor = jsn_sensor_ns.class_("JSNSensor", cg.PollingComponent, sensor.Sensor)

PLATFORM_SCHEMA = cv.PLATFORM_SCHEMA.extend({
    cv.Required("uart_id"): cv.use_id(uart.UARTComponent),
}).extend(sensor.SENSOR_SCHEMA.schema)

async def to_code(config):
    uart_ = await cg.get_variable(config["uart_id"])
    var = cg.new_Pvariable(config["name"], uart_)
    await sensor.register_sensor(var, config)
