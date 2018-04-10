#include "basic_io_test.hpp"
#include "wav_test.hpp"
#include "beamformer_test.hpp"
#include "fir_test.hpp"

int main() {
    assert(basic_io_test() >= 0);
    assert(wav_test() >= 0);
    assert(beamformer_test() >= 0);
    assert(fir_test() >= 0);

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
