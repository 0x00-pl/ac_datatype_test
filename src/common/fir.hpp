#include "vm.hpp"

namespace pl_vm {
using namespace std;

template<size_t N, typename Tin, typename Tout=Tin, typename Tsum=Tin, typename Tmul=Tsum, typename Tparam=Tin>
class fir_unit: public hd_unit {
public:
    weak_ptr<hd_wire<Tin>> input;
    weak_ptr<hd_wire<Tout>> output;
    Tparam (&param)[N];
    optional<Tin> buffer[N];
    size_t p_buffer;

    fir_unit(
        weak_ptr<hd_wire<Tin>> in_w,
        weak_ptr<hd_wire<Tout>> out_w,
        Tparam (&param_)[N])
        :input {in_w}, output {out_w}, param {param_}, p_buffer(0) {}

    void tick(int debug_level) override {
        auto data = input.lock()->data;
        p_buffer = p_offset(1);
        buffer[p_offset(0)] = data;
        Tsum sum = 0;
        for(size_t i=0; i<N; i++) {
            auto bv = buffer[p_offset(-i)];
            if(bv) {
                Tmul m = bv.value() * param[N-i-1];
                sum = sum + m;
            } else {
                output.lock()->data = {};
                return;
            }
        }
        output.lock()->data = sum;
    }

    size_t p_offset(size_t offset) {
        return (N + p_buffer + offset) % N;
    }
};
}
