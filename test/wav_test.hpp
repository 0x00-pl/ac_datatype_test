#include "common/wav.hpp"

using namespace pl_vm;
using namespace std;

int wav_test() {
    WavHeader h;
    wav_data d = read_wav("data/input.wav", h);
    assert(h.bit == 16);
    uint16_t* data = d.u16;

    cout <<"wav sample size: "<< h.data_size<<endl;

    write_wav("data/out/input.wav", h, d);

    return -system("diff data/input.wav data/out/input.wav");
}
