import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import i2c
from esphome.const import CONF_ID

AUTO_LOAD = ["sensor"]
DEPENDENCIES = ["i2c"]
MULTI_CONF = True

ds3502_ns = cg.esphome_ns.namespace("ds3502")
DS3502Component = ds3502_ns.class_("DS3502Component", cg.Component, i2c.I2CDevice)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(DS3502Component),
}).extend(i2c.i2c_device_schema(0x28)).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)
