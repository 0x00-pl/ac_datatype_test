#pragma once
#include <cmath>
#include <array>
#include <algorithm>
#include "vm.hpp"

namespace pl_vm {
using namespace std;

template<typename T>
class point3d : public array<T,3> {
public:
    point3d() : array<T,3> {0,0,0} {}
    template<typename Tv>
    T dot(const point3d<Tv>& a) {
        return a[0] **this[0] + a[1] **this[1] + a[2] **this[2];
    }
    template<typename Tv>
    point3d operator*(const Tv& a) {
        return {*this[0]*a, *this[1]*a, *this[2]*a};
    }
};

template<typename T, size_t N>
array<point3d<T>, N> make_ula(point3d<T> dir) {
    array<point3d<T>, N> ret;
    for(size_t i=0; i<N; i++) {
        ret[i] = dir * i;
    }
    return ret;
}
template<typename T, size_t N>
array<point3d<T>, N> make_uca(float radius) {
    array<point3d<T>, N> ret;
    for(size_t i=0; i<N; i++) {
        ret[i] = {sinf(radius * i/M_PI), cosf(radius * i/M_PI), 0};
    }
    return ret;
}

template<typename T>
array<point3d<T>, 7> make_default_uca()
{
    array<point3d<T>, 7> res;
    res[0] = {-0.05f, 0.0866f};
    res[1] = {0.05f, 0.0866f};
    res[2] = {-0.10f, 0.000f};
    res[3] = {0.0f, 0.0f};
    res[4] = {0.10f, 0.000f};
    res[5] = {-0.05f, -0.0866f};
    res[6] = {0.05f, -0.0866f};
    return res;
}

template<typename T, size_t N>
array<size_t, N> get_delay_config(
    const array<point3d<T>,N>& sensors, double angle,
    double Fs = 48000, double sound_speed_m = 343
) {
    array<double, N> tmp;
    point3d<T> angle_vector {sinf(angle), cosf(angle)};
    angle_vector = angle_vector * (Fs/sound_speed_m);
    for(size_t i=0; i<N; i++) {
        tmp[i] = sensors[i] * angle_vector;
    }
    double delay_min = *min_element(begin(tmp), end(tmp));
    array<size_t, N> ret;
    for(size_t i=0; i<N; i++) {
        ret[i] = lround(tmp[i] - delay_min);
    }
    return ret;
}
}
