#ifndef UTILS_H
#define UTILS_H

#include <bitset>
#include <cmath>
#include <cstdint>
#include <stdexcept>

#ifdef ARDUINO
  #include <Arduino.h>
  #include <ArduinoJson.h>
#else
  #include <iostream>
  #include <string>
  #include <nlohmann/json.hpp>
  #include <libudev.h>
#endif

namespace Calcs {
float map_float(float x, float in_min, float in_max, float out_min, float out_max);
float constrain_float(float x, float min, float max);
float calc_dead_band(float x, float max_output, float dead_band);
float milli_to_single(float x);
float rad_to_deg(float x);
float calc_hypotenuse_angle(float a, float b);
float calc_shock_travel(float alpha, float a, float b);
float calc_alpha(float a, float b, float c);
float two_decimals(float value);

#ifdef ARDUINO
uint8_t calc_checksum(const String &data);
bool verify_checksum(const String &data);
#else
uint8_t calc_checksum(const std::string &data);
bool verify_checksum(const std::string &data);
std::string find_device_by_description(const std::string &description);
std::string find_device_by_id(const std::string &vendor_id, const std::string &product_id);
#endif
}  // namespace Calcs
#endif  // UTILS_H