import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart, sensor
from esphome.const import CONF_ID, UNIT_CENTIMETER

DEPENDENCIES = ["uart"]

jsn_ns = cg.esphome_ns.namespace("jsn_sensor")
JSNSensor = jsn_ns.class_("JSNSensor", sensor.Sensor, cg.Component)

CONFIG_SCHEMA = sensor.sensor_schema(
    unit_of_measurement=UNIT_CENTIMETER,
    accuracy_decimals=1,
).extend(
    {
        cv.GenerateID(): cv.declare_id(JSNSensor),
        cv.Required("uart_id"): cv.use_id(uart.UARTComponent),
    }
)

async def to_code(config):
    uart_comp = await cg.get_variable(config["uart_id"])
    sens = cg.new_Pvariable(config[CONF_ID], uart_comp)
    await cg.register_component(sens, config)
    await sensor.register_sensor(sens, config)
