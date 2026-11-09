import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import CONF_ID, UNIT_EMPTY, ICON_EMPTY
from . import DS3502Component, ds3502_ns

DS3502Sensor = ds3502_ns.class_('DS3502Sensor', cg.PollingComponent)

CONF_DS3502_ID = "ds3502_id"

CONFIG_SCHEMA = sensor.sensor_schema(UNIT_EMPTY, ICON_EMPTY, 1).extend({
    cv.Required(CONF_DS3502_ID): cv.use_id(DS3502Component),
    cv.Required(CONF_ID): cv.declare_id(DS3502Sensor),
}).extend(cv.polling_component_schema('60s'))

def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)
    yield sensor.register_sensor(var, config)
