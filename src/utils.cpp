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
    if (abs(x) < dead_band) {
        return 0;
    }
    int sign = x > 0 ? 1 : -1;
    float abs_temp = map_float(abs(x), dead_band, max_output, 0, max_output);
    float scaled_temp = sign * abs_temp;
    return scaled_temp;
}
float milli_to_single(float x) { return x / 1e3; }
float rad_to_deg(float x) { return x * 180 / PI; }
float calc_hypotenuse_angle(float a, float b) { return atan2(b, a); }
float calc_shock_travel(float alpha, float a, float b) {
    // Using the Law of Cosines to calculate the length of side C
    float c = sqrt(a * a + b * b - 2 * a * b * cos(alpha));
    return c;
}
float calc_alpha(float a, float b, float c) {
    // Using the Law of Cosines to calculate the angle alpha
    float alpha = acos((a * a + b * b - c * c) / (2 * a * b));
    return alpha;
}
float two_decimals(float value) { return (round(value * 100) / 100); }

}  // namespace Calcs
