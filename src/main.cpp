#include <bitset>
#include <iostream>
#include "ac_datatype/ac_fixed.h"
using namespace std;

int main() {
    std::string bit_string = "11000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000010";
    std::bitset<320> bs {bit_string};
    int buff[10] = {1,2,3};
    bs = *(bitset<320>*)&buff;

    ac_fixed<160,1, true> ac16 = -0.50;
    std::bitset<160> b16;
    b16 = *(bitset<160>*)&ac16;
    cout<<b16<<endl<<ac16.to_string(AC_BIN,true)<<endl<<ac16<<endl;
}
