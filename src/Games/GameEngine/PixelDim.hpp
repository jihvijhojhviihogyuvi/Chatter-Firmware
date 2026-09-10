#ifndef CIRCUITPET_FIRMWARE_PIXELDIM_HPP
#define CIRCUITPET_FIRMWARE_PIXELDIM_HPP

#include <math.h>

namespace std {
using ::acos;
using ::acosh;
using ::asin;
using ::asinh;
using ::atan;
using ::atanh;
using ::cos;
using ::cosh;
using ::exp;
using ::exp2;
using ::fma;
using ::fmax;
using ::fmin;
using ::log;
using ::pow;
using ::sin;
using ::sinh;
using ::sqrt;
using ::tan;
using ::tanh;
using ::trunc;
inline float log2(float value) { return ::log2f(value); }
inline double log2(double value) { return ::log2(value); }
inline long double log2(long double value) { return ::log2(value); }
inline float nextafter(float value, float target) { return ::nextafterf(value, target); }
inline double nextafter(double value, double target) { return ::nextafter(value, target); }
inline float round(float value) { return ::roundf(value); }
inline double round(double value) { return ::round(value); }
inline long double round(long double value) { return ::round(value); }
}

#include <glm.h>

typedef glm::i16vec2 PixelDim;

#endif //CIRCUITPET_FIRMWARE_PIXELDIM_HPP
