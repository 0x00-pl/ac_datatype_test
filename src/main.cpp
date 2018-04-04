#include <bitset>
#include <iostream>
#include "ac_datatype/ac_fixed.h"
#include "vm.hpp"
#include "io_unit.hpp"

using namespace std;
using namespace pl_vm;

void basic_io(){
    vm v;
    auto w1 = v.make_wire<hd_wire<int>>();
    int data[] = {1,2,3,4};
    v.make_unit<input_unit<int>>(w1, data, size(data));
    v.make_unit<print_unit<int>>(w1);
    for(size_t i=0; i<5; i++){
        v.tick(0);
    }
}

int main() {
    basic_io();
//     std::string bit_string = "11000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000010";
//     std::bitset<320> bs {bit_string};
//     int buff[10] = {1,2,3};
//     bs = *(bitset<320>*)&buff;
//
//     ac_fixed<160,1, true> ac16 = -0.50;
//     std::bitset<160> b16;
//     b16 = *(bitset<160>*)&ac16;
//     cout<<b16<<endl<<ac16.to_string(AC_BIN,true)<<endl<<ac16<<endl;
}
