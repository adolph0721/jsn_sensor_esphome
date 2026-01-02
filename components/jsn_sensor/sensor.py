import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart, sensor
from esphome.const import (
    CONF_ID,
    UNIT_CENTIMETER,
    ICON_ARROW_EXPAND_VERTICAL,
)

CODEOWNERS = ["@adolph0721"]
DEPENDENCIES = ["uart"]

jsn_sensor_ns = cg.esphome_ns.namespace("jsn_sensor")
JSNSensor = jsn_sensor_ns.class_(
    "JSNSensor", cg.PollingComponent, sensor.Sensor
)

CONFIG_SCHEMA = (
    sensor.sensor_schema(
        unit_of_measurement=UNIT_CENTIMETER,
        icon=ICON_ARROW_EXPAND_VERTICAL,
        accuracy_decimals=1,
    )
    .extend(
        {
            cv.GenerateID(): cv.declare_id(JSNSensor),
            cv.Required("uart_id"): cv.use_id(uart.UARTComponent),
        }
    )
    .extend(cv.polling_component_schema("500ms"))
)

async def to_code(config):
    uart_comp = await cg.get_variable(config["uart_id"])
    var = cg.new_Pvariable(config[CONF_ID], uart_comp)

    await cg.register_component(var, config)
    await sensor.register_sensor(var, config)
