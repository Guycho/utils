
#include <Arduino.h>

namespace DataTypes
{
struct TelemetryData {
    bool arm_state;
    uint8_t steering_mode;
    uint8_t drive_mode;
    uint8_t battery_status;
    uint16_t motors_rpm[4];
    float battery_voltage;
    float g_force_x;
    float g_force_y;
    float rotation_rate_z;
    float motors_throttle[4];
    float steering_values[4];
};
struct RemoteControllerData {
    float throttle = 0;
    float steering = 0;
    bool left_arrow = 0;
    bool right_arrow = 0;
    bool up_arrow = 0;
    bool down_arrow = 0;
    bool sel = 0;
    bool ch = 0;
    bool plus = 0;
    bool minus = 0;
    bool left_trim_l = 0;
    bool left_trim_r = 0;
    bool right_trim_l = 0;
    bool right_trim_r = 0;
    bool edge_switch = 0;
    bool bottom_switch = 0;
    bool new_data = 0;
};

}  // namespace DataTypes