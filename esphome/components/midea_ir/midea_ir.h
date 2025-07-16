#pragma once

#include "esphome/components/climate_ir/climate_ir.h"
#include "midea_data.h"

namespace esphome {
namespace midea_ir {

// Temperature
const uint8_t MIDEA_TEMPC_MIN = 17;  // Celsius
const uint8_t MIDEA_TEMPC_MAX = 30;  // Celsius
const uint8_t MIDEA_TEMPF_MIN = 62;  // Fahrenheit
const uint8_t MIDEA_TEMPF_MAX = 86;  // Fahrenheit

climate::ClimateTraits traits() override {
  auto traits = climate_ir::ClimateIR::traits();

  traits.set_supported_modes({
    climate::CLIMATE_MODE_COOL,
    climate::CLIMATE_MODE_DRY,
    climate::CLIMATE_MODE_FAN_ONLY,
    climate::CLIMATE_MODE_HEAT_COOL,  // We will rename this below
  });

  traits.set_custom_mode_names({
    {climate::CLIMATE_MODE_HEAT_COOL, "Auto"},
  });

  traits.set_supported_presets({
    climate::CLIMATE_PRESET_NONE,
    climate::CLIMATE_PRESET_SLEEP,
    climate::CLIMATE_PRESET_BOOST,
  });

  traits.set_supported_fan_modes({
    climate::CLIMATE_FAN_AUTO,
    climate::CLIMATE_FAN_LOW,
    climate::CLIMATE_FAN_MEDIUM,
    climate::CLIMATE_FAN_HIGH,
  });

  traits.set_supported_swing_modes({});  // No swing

  return traits;
}

class MideaIR : public climate_ir::ClimateIR {
 public:
  MideaIR()
      : climate_ir::ClimateIR(
    MIDEA_TEMPC_MIN, MIDEA_TEMPC_MAX, 1.0f, true, false,  // ❌ disable HEAT
    {climate::CLIMATE_FAN_AUTO, climate::CLIMATE_FAN_LOW, climate::CLIMATE_FAN_MEDIUM,
     climate::CLIMATE_FAN_HIGH},
    {},  // ❌ remove all swing modes
    {climate::CLIMATE_PRESET_NONE, climate::CLIMATE_PRESET_SLEEP, climate::CLIMATE_PRESET_BOOST}) {}

  /// Override control to change settings of the climate device.
  void control(const climate::ClimateCall &call) override;

  /// Set use of Fahrenheit units
  void set_fahrenheit(bool value) {
    this->fahrenheit_ = value;
    this->temperature_step_ = value ? 0.5f : 1.0f;
  }

 protected:
  /// Transmit via IR the state of this climate controller.
  void transmit_state() override;
  void transmit_(MideaData &data);
  /// Handle received IR Buffer
  bool on_receive(remote_base::RemoteReceiveData data) override;
  bool on_midea_(const MideaData &data);
  bool fahrenheit_{false};
  bool swing_{false};
  bool boost_{false};
};

}  // namespace midea_ir
}  // namespace esphome
