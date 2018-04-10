#include "common/vm.hpp"
#include "common/io_unit.hpp"
#include "common/vcd_tree.hpp"
#include "common/fir.hpp"

using namespace pl_vm;

int fir_test() {

    vm v;
    auto in_w = v.make_wire<hd_wire<uint16_t>>();
    auto out_w = v.make_wire<hd_wire<uint16_t>>();

    uint16_t data[1024] = {0};
    for(size_t i=0; i<1024; i++) {
        data[i]=i;
    }
    auto data_gen = v.make_unit<input_unit<uint16_t>>(in_w, data, 1024);

    vcd_tree vt("fir");
    auto in_vcd = make_shared<vcd_tree>("in_w");
    in_vcd->registry_wire(in_w, "0");
    vt.add_sub_tree(in_vcd);
    auto out_vcd = make_shared<vcd_tree>("out_w");
    out_vcd->registry_wire(out_w, "0");
    vt.add_sub_tree(out_vcd);
    ofstream fvcd("data/out/fir.vcd");
    vt.exports_header(fvcd);
    size_t time=0;

    uint16_t param[16] = {0, 1, 0};
    auto b = v.make_unit<fir_unit<16, uint16_t>>(in_w, out_w, param);

    do {
        v.tick(0);
        vt.export_var(fvcd, time++);
    } while(out_w.lock()->data || time<100);

    return 0;
}
