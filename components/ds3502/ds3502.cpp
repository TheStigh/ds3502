#include "ds3502.h"
#include "esphome/core/log.h"

namespace esphome {
namespace ds3502 {

static const char *const TAG = "ds3502";

void DS3502Component::setup() {
  ESP_LOGCONFIG(TAG, "Setting up DS3502...");
  
  // Try to write to the wiper register to verify the device is connected
  if (!this->write_byte(DS3502_REG_WIPER, 0x00)) {
    ESP_LOGE(TAG, "Communication with DS3502 failed!");
    this->mark_failed();
    return;
  }
  
  ESP_LOGD(TAG, "DS3502 initialized successfully");
}

void DS3502Component::dump_config() {
  ESP_LOGCONFIG(TAG, "DS3502:");
  LOG_I2C_DEVICE(this);
  
  if (this->is_failed()) {
    ESP_LOGE(TAG, "Communication failed");
  }
}

void DS3502Component::set_wiper(uint8_t value) {
  // The DS3502 has 7-bit resolution (0-127)
  if (value > 127)
    value = 127;
    
  ESP_LOGD(TAG, "Setting wiper to %u", value);
  this->write_byte(DS3502_REG_WIPER, value);
}

uint8_t DS3502Component::get_wiper() {
  uint8_t value = 0;
  this->read_byte(DS3502_REG_WIPER, &value);
  return value;
}

void DS3502Output::setup() {
  if (this->parent_ == nullptr) {
    ESP_LOGE(TAG, "DS3502Output parent not set!");
    this->mark_failed();
    return;
  }
  
  ESP_LOGCONFIG(TAG, "Setting up DS3502 output");
}

void DS3502Output::dump_config() {
  ESP_LOGCONFIG(TAG, "DS3502 Output:");
  ESP_LOGCONFIG(TAG, "  Component parent %s", this->parent_ ? "set" : "NOT SET");
}

void DS3502Output::write_state(float state) {
  if (this->parent_ == nullptr) {
    ESP_LOGW(TAG, "No parent set for DS3502Output!");
    return;
  }
  
  // Map state (0.0-1.0) to wiper value (0-127)
  int wiper_value = static_cast<int>(state * 127.0f);
  
  // Clamp value to valid range
  wiper_value = std::max(0, std::min(127, wiper_value));
  
  ESP_LOGD(TAG, "Setting output: %.2f%% (wiper: %d)", state * 100.0f, wiper_value);
  this->parent_->set_wiper(wiper_value);
}



void DS3502Sensor::setup() {
  if (this->parent_ == nullptr) {
    ESP_LOGE(TAG, "DS3502Sensor parent not set!");
    this->mark_failed();
    return;
  }
  
  ESP_LOGCONFIG(TAG, "Setting up DS3502 output");
}

void DS3502Sensor::dump_config() {
  ESP_LOGCONFIG(TAG, "DS3502 Sensor:");
  ESP_LOGCONFIG(TAG, "  Component parent %s", this->parent_ ? "set" : "NOT SET");
}

void DS3502Sensor::update() {
  if (this->parent_ == nullptr) {
    ESP_LOGW(TAG, "No parent set for DS3502Input!");
    return;
  }
  this->publish_state(this->parent_->get_wiper());
}

}  // namespace ds3502
}  // namespace esphome
