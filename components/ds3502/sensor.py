import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import CONF_ID, UNIT_EMPTY, ICON_EMPTY
from . import DS3502Component, ds3502_ns

DS3502Sensor = ds3502_ns.class_('DS3502Sensor', cg.PollingComponent)

CONF_DS3502_ID = "ds3502_id"

CONFIG_SCHEMA = sensor.sensor_schema(DS3502Sensor).extend({
    cv.Required(CONF_DS3502_ID): cv.use_id(DS3502Component),
    cv.Required(CONF_ID): cv.declare_id(DS3502Sensor),
}).extend(cv.polling_component_schema('60s'))

async to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await sensor.register_sensor(var, config)
    
    parent = await cg.get_variable(config[CONF_DS3502_ID])
    cg.add(var.set_parent(parent))
