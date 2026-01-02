import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, uart

CONF_UART_ID = "uart_id"

# 建立 C++ namespace 與 class
jsn_sensor_ns = cg.esphome_ns.namespace("jsn_sensor")
JSNSensor = jsn_sensor_ns.class_(
    "JSNSensor", cg.PollingComponent, sensor.Sensor
)

# 用最新的 cv.Schema 定義
PLATFORM_SCHEMA = cv.Schema({
    cv.Required(CONF_UART_ID): cv.use_id(uart.UARTComponent),
    cv.Required(sensor.CONF_ID): cv.declare_id(JSNSensor),
})

async def to_code(config):
    uart_var = await cg.get_variable(config[CONF_UART_ID])
    var = cg.new_Pvariable(config[sensor.CONF_ID], uart_var)
    await sensor.register_sensor(var, config)
