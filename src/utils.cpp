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

std::string find_device_by_id(const std::string &vendor_id, const std::string &product_id) {
    struct udev *udev = udev_new();
    if (!udev) {
        throw std::runtime_error("Failed to create udev object");
    }

    // First, find the USB device
    struct udev_enumerate *enumerate = udev_enumerate_new(udev);
    udev_enumerate_add_match_subsystem(enumerate, "usb");
    udev_enumerate_scan_devices(enumerate);

    struct udev_list_entry *devices = udev_enumerate_get_list_entry(enumerate);
    struct udev_list_entry *entry;

    std::string tty_device;
    udev_list_entry_foreach(entry, devices) {
        const char *path = udev_list_entry_get_name(entry);
        struct udev_device *dev = udev_device_new_from_syspath(udev, path);

        struct udev_device *parent = udev_device_get_parent(dev);
        if (parent) {
            const char *dev_vendor = udev_device_get_sysattr_value(parent, "idVendor");
            const char *dev_product = udev_device_get_sysattr_value(parent, "idProduct");
            if (dev_vendor && dev_product) {
                std::string dev_vendor_str(dev_vendor);
                std::string dev_product_str(dev_product);
                std::cout << "Found device: Vendor ID: " << dev_vendor_str << ", Product ID: " << dev_product_str << std::endl;
                if (vendor_id == dev_vendor_str && product_id == dev_product_str) {
                    // Now, find the corresponding TTY device
                    struct udev_enumerate *tty_enumerate = udev_enumerate_new(udev);
                    udev_enumerate_add_match_subsystem(tty_enumerate, "tty");
                    udev_enumerate_scan_devices(tty_enumerate);

                    struct udev_list_entry *tty_devices = udev_enumerate_get_list_entry(tty_enumerate);
                    struct udev_list_entry *tty_entry;

                    udev_list_entry_foreach(tty_entry, tty_devices) {
                        const char *tty_path = udev_list_entry_get_name(tty_entry);
                        struct udev_device *tty_dev = udev_device_new_from_syspath(udev, tty_path);

                        struct udev_device *tty_parent = udev_device_get_parent_with_subsystem_devtype(tty_dev, "usb", "usb_device");
                        if (tty_parent) {
                            const char *tty_dev_vendor = udev_device_get_sysattr_value(tty_parent, "idVendor");
                            const char *tty_dev_product = udev_device_get_sysattr_value(tty_parent, "idProduct");
                            if (tty_dev_vendor && tty_dev_product) {
                                std::string tty_dev_vendor_str(tty_dev_vendor);
                                std::string tty_dev_product_str(tty_dev_product);
                                if (vendor_id == tty_dev_vendor_str && product_id == tty_dev_product_str) {
                                    const char *dev_node = udev_device_get_devnode(tty_dev);
                                    if (dev_node) {
                                        tty_device = dev_node;
                                        std::cout << "TTY device node: " << tty_device << std::endl;
                                        udev_device_unref(tty_dev);
                                        break;
                                    }
                                }
                            }
                        }
                        udev_device_unref(tty_dev);
                    }

                    udev_enumerate_unref(tty_enumerate);
                    if (!tty_device.empty()) {
                        udev_device_unref(dev);
                        udev_enumerate_unref(enumerate);
                        udev_unref(udev);
                        return tty_device;
                    } else {
                        std::cout << "No corresponding TTY device found for USB device with Vendor ID: " << dev_vendor_str << ", Product ID: " << dev_product_str << std::endl;
                    }
                }
            } else {
                std::cout << "Device vendor or product ID is nullptr" << std::endl;
            }
        } else {
            std::cout << "Parent device is nullptr" << std::endl;
        }
        udev_device_unref(dev);
    }

    udev_enumerate_unref(enumerate);
    udev_unref(udev);
    throw std::runtime_error("Device with the specified vendor ID and product ID not found");
}

}  // namespace Calcs