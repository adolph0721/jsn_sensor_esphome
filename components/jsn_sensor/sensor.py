import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, uart

jsn_sensor_ns = cg.esphome_ns.namespace("jsn_sensor")
JSNSensor = jsn_sensor_ns.class_("JSNSensor", cg.PollingComponent, sensor.Sensor)

CONFIG_SCHEMA = cv.Schema({
    cv.Required("uart_id"): cv.use_id(uart.UARTComponent),
    cv.Required("name"): cv.string,
    cv.Optional("id"): cv.declare_id(JSNSensor),
    cv.Optional("unit_of_measurement"): cv.string,
    cv.Optional("icon"): cv.icon,
    cv.Optional("accuracy_decimals"): cv.int_,
    cv.Optional("device_class"): cv.string,
    cv.Optional("state_class"): cv.string,
    cv.Optional("force_update", default=True): cv.boolean,
    cv.Optional("disabled_by_default", default=False): cv.boolean,
}).extend({})

async def to_code(config):
    uart_ = await cg.get_variable(config["uart_id"])

    if "id" in config:
        var = cg.new_Pvariable(config["id"], uart_)
    else:
        var = cg.new_Pvariable(config["name"], uart_)

    if "unit_of_measurement" in config:
        var.set_unit_of_measurement(config["unit_of_measurement"])
    if "accuracy_decimals" in config:
        var.set_accuracy_decimals(config["accuracy_decimals"])
    if "icon" in config:
        var.set_icon(config["icon"])
    if "device_class" in config:
        var.set_device_class(config["device_class"])
    if "state_class" in config:
        var.set_state_class(config["state_class"])

    await sensor.register_sensor(var, config)
