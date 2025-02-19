#include "utils.h"

namespace Calcs {
float map_float(float x, float in_min, float in_max, float out_min, float out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float constrain_float(float x, float min, float max) {
    if (x < min) {
        return min;
    } else if (x > max) {
        return max;
    } else {
        return x;
    }
}

float calc_dead_band(float x, float max_output, float dead_band) {
    if (std::abs(x) < dead_band) {
        return 0;
    }
    int sign = x > 0 ? 1 : -1;
    float abs_temp = map_float(std::abs(x), dead_band, max_output, 0, max_output);
    float scaled_temp = sign * abs_temp;
    return scaled_temp;
}

float milli_to_single(float x) { return x / 1e3; }

float rad_to_deg(float x) { return x * 180 / M_PI; }

float calc_hypotenuse_angle(float a, float b) { return std::atan2(b, a); }

float calc_shock_travel(float alpha, float a, float b) {
    // Using the Law of Cosines to calculate the length of side C
    float c = std::sqrt(a * a + b * b - 2 * a * b * std::cos(alpha));
    return c;
}

float calc_alpha(float a, float b, float c) {
    // Using the Law of Cosines to calculate the angle alpha
    float alpha = std::acos((a * a + b * b - c * c) / (2 * a * b));
    return alpha;
}

float two_decimals(float value) { return (std::round(value * 100) / 100); }

uint8_t calc_checksum(const std::string &data) {
    uint8_t checksum = 0;
    for (char c : data) {
        checksum ^= c;
    }
    return checksum;
}

bool verify_checksum(const std::string &data) {
    nlohmann::json m_json_data = nlohmann::json::parse(data);
    // Extract the checksum from the JSON document
    uint8_t received_checksum = m_json_data["c"];
    // Remove the checksum from the JSON document
    m_json_data.erase("c");
    std::string json = m_json_data.dump();

    uint8_t calculated_checksum = Calcs::calc_checksum(json);
    if (received_checksum != calculated_checksum) {
        return false;  // Checksum mismatch;
    }
    return true;
}

std::string find_device_by_description(const std::string &description) {
    struct udev *udev = udev_new();
    if (!udev) {
        throw std::runtime_error("Failed to create udev object");
    }

    struct udev_enumerate *enumerate = udev_enumerate_new(udev);
    udev_enumerate_add_match_subsystem(enumerate, "tty");
    udev_enumerate_scan_devices(enumerate);

    struct udev_list_entry *devices = udev_enumerate_get_list_entry(enumerate);
    struct udev_list_entry *entry;

    udev_list_entry_foreach(entry, devices) {
        const char *path = udev_list_entry_get_name(entry);
        struct udev_device *dev = udev_device_new_from_syspath(udev, path);

        struct udev_device *parent = udev_device_get_parent(dev);
        if (parent) {
            const char *dev_desc = udev_device_get_sysattr_value(parent, "product");
            if (dev_desc && description == dev_desc) {
                const char *dev_node = udev_device_get_devnode(dev);
                if (dev_node) {
                    std::string device_name(dev_node);
                    udev_device_unref(dev);
                    udev_enumerate_unref(enumerate);
                    udev_unref(udev);
                    return device_name;
                }
            }
        }
        udev_device_unref(dev);
    }

    udev_enumerate_unref(enumerate);
    udev_unref(udev);
    throw std::runtime_error("Device with the specified description not found");
}
}  // namespace Calcs