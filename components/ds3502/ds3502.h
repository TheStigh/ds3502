#pragma once

#include "esphome/core/component.h"
#include "esphome/components/output/float_output.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/i2c/i2c.h"

namespace esphome {
namespace ds3502 {

// DS3502 register addresses
static const uint8_t DS3502_REG_WIPER = 0x00;
static const uint8_t DS3502_REG_CONFIG = 0x02;

class DS3502Component : public Component, public i2c::I2CDevice {
 public:
  void setup() override;
  void dump_config() override;
  float get_setup_priority() const override { return setup_priority::HARDWARE; }
  
  void set_wiper(uint8_t value);
  uint8_t get_wiper();
};

class DS3502Output : public output::FloatOutput, public Component {
 public:
  void set_parent(DS3502Component *parent) { parent_ = parent; }
  
  void setup() override;
  void dump_config() override;
  float get_setup_priority() const override { return setup_priority::HARDWARE; }
  
 protected:
  void write_state(float state) override;
  DS3502Component *parent_{nullptr};
};

class DS3502Sensor : public sensor::Sensor, public PollingComponent {
 public:
  void set_parent(DS3502Component *parent) { parent_ = parent; }

  void setup() override;
  void update() override;
  void dump_config() override;

protected:
  DS3502Component *parent_{nullptr};
};

}  // namespace ds3502
}  // namespace esphome
