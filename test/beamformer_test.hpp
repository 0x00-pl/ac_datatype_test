#include "common/vm.hpp"
#include "common/beamformer.hpp"
#include "common/io_unit.hpp"
#include "common/vcd_tree.hpp"

using namespace pl_vm;

int beamformer_test() {
    const size_t Nin = 4;
    const size_t Nout = 8;

    vm v;
    weak_ptr<hd_wire<uint16_t>> in_w[Nin] = {
        v.make_wire<hd_wire<uint16_t>>(),
        v.make_wire<hd_wire<uint16_t>>(),
        v.make_wire<hd_wire<uint16_t>>(),
        v.make_wire<hd_wire<uint16_t>>()
    };
    weak_ptr<hd_wire<uint16_t>> out_w[Nout] = {
        v.make_wire<hd_wire<uint16_t>>(),
        v.make_wire<hd_wire<uint16_t>>(),
        v.make_wire<hd_wire<uint16_t>>(),
        v.make_wire<hd_wire<uint16_t>>(),
        v.make_wire<hd_wire<uint16_t>>(),
        v.make_wire<hd_wire<uint16_t>>(),
        v.make_wire<hd_wire<uint16_t>>(),
        v.make_wire<hd_wire<uint16_t>>()
    };

    uint16_t data[1024] = {0};
    for(size_t i=0; i<1024; i++) {
        data[i]=i;
    }
    auto data_gen = {
        v.make_unit<input_unit<uint16_t>>(in_w[0], data, 1024),
        v.make_unit<input_unit<uint16_t>>(in_w[1], data, 1024),
        v.make_unit<input_unit<uint16_t>>(in_w[2], data, 1024),
        v.make_unit<input_unit<uint16_t>>(in_w[3], data, 1024),
    };

    vcd_tree vt("beamformer");
    auto in_vcd = make_shared<vcd_tree>("in_w");
    in_vcd->registry_wire_array(in_w);
    vt.add_sub_tree(in_vcd);
    auto out_vcd = make_shared<vcd_tree>("out_w");
    out_vcd->registry_wire_array(out_w);
    vt.add_sub_tree(out_vcd);
    ofstream fvcd("data/out/beamformer.vcd");
    vt.exports_header(fvcd);
    size_t time=0;

    size_t delay[Nout][Nin] = {0};
    auto b = v.make_unit<beamformer_unit<uint16_t, Nin, uint16_t, Nout, 64, uint32_t>>(in_w, out_w, delay);

    do {
        v.tick(0);
        vt.export_var(fvcd, time++);
    } while(out_w[0].lock()->data || time<100);

    return 0;
}
