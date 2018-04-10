#include "vm.hpp"

namespace pl_vm {
using namespace std;

template<typename Tout>
class sine_unit: public hd_unit {
public:
    weak_ptr<hd_wire<Tout>> output;
    double scale, bias, freq, phase;
    size_t sample_rate;
    size_t it;

    sine_unit(
        weak_ptr<hd_wire<Tout>> out_w,
        double freq_,
        size_t sample_rate_=48000,
        double scale_ = 1,
        double bias_=0,
        double phase_=0)
        : output {out_w}, freq {freq_}, sample_rate {sample_rate_},
    scale(scale_), bias {bias_}, phase {phase_} {}

    void tick(int debug_level) override {
        output.lock()->data = scale * sin(it *2 *  M_PI / sample_rate * freq  + phase) + bias;
        it++;
    }
};
}
