#pragma once
#include <bitset>
#include <iostream>
#include <fstream>
#include "ac_datatype/ac_fixed.h"
#include "common/vm.hpp"
#include "common/io_unit.hpp"
#include "common/vcd_tree.hpp"

using namespace std;
using namespace pl_vm;

int basic_io_test(){
    // vm
    vm v;
    auto w1 = v.make_wire<hd_wire<int>>();
    auto w2 = v.make_wire<hd_wire<char>>();
    int data[] = {1,0,2,3};
    char data2[] = {1,0,2,3,4,5,6};
    v.make_unit<input_unit<int>>(w1, data, size(data));
    v.make_unit<input_unit<char>>(w2, data2, size(data2));
    v.make_unit<print_unit<int>>(w1);

    // vt
    vcd_tree vt("top");
    size_t time = 0;
    vt.registry_wire(w1, "w1");
    vt.registry_wire(w2, "w2");
    ofstream vt_out("data/out/basic_io.vcd");
    vt.exports_header(vt_out);

    // loop
    for(size_t i=0; i<15; i++){
        v.tick(0);
        vt.export_var(vt_out, time++);
    }

    return 0;
}
