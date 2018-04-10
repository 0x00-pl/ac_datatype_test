#include "vm.hpp"


namespace pl_vm {
using namespace std;

template<typename Tin, size_t Nin, typename Tout, size_t Nout, size_t Nbuf, typename Tcal>
class beamformer_unit: public hd_unit {
public:
    weak_ptr<hd_wire<Tin>> (&input)[Nin];
    weak_ptr<hd_wire<Tout>> (&output)[Nout];
    size_t (&delay)[Nout][Nin];
    Tin buffer[Nbuf][Nin];
    size_t p_buffer;

    beamformer_unit(
        weak_ptr<hd_wire<Tin>> (&in_w)[Nin],
        weak_ptr<hd_wire<Tout>> (&out_w)[Nout],
        size_t (&delay_)[Nout][Nin])
        :input {in_w}, output {out_w}, delay {delay_}, buffer {0}, p_buffer(0) {}

    void tick(int debug_level) override {
        (void)debug_level;
        bool input_unset = false;
        for(size_t i=0; i<Nin; i++) {
            auto data = input[i].lock()->data;
            if(!data) {
                input_unset = true;
                buffer[p_offset(0)][i] = 0;
                continue;
            }
            buffer[p_offset(0)][i] = data.value();
        }
        if(input_unset) {
            for(size_t oc=0; oc<Nout; oc++) {
                output[oc].lock()->data = {};
            }
        } else {
            for(size_t oc=0; oc<Nout; oc++) {
                Tcal sum {0};
                for(size_t ic=0; ic<Nin; ic++) {
                    sum += buffer[p_offset(-delay[oc][ic])][ic];
                }
                output[oc].lock()->data = sum/Nin;
            }
        }
        p_buffer = p_offset(1);
    }

    size_t p_offset(size_t offset) {
        return (Nbuf + p_buffer + offset) % Nbuf;
    }
};
}
