#include <iostream>
#include <fstream>
#include <cstring>
#include "vm.hpp"

namespace pl_vm
{
using namespace std;

struct WavHeader {
    char str_riff[4]; // "riff"
    unsigned int size;
    char str_wave[4]; // "WAVE"
    char str_fmt[4]; // "fmt "
    unsigned int fmt_size; //
    unsigned short format; //
    unsigned short channels;
    unsigned int sample_rate;
    unsigned int bytes_per_second; //
    unsigned short block_size;
    unsigned short bit; //
    char str_data[4]; // "data"
    unsigned int data_size; //
};

template<typename T>
T* read_wav_data(istream& w, size_t channel_length, size_t data_length) {
    T* ret = new T[channel_length*data_length];
    for(size_t i=0; i<data_length; i++) {
        for(size_t ch=0; ch<channel_length; ch++) {
            T buf;
            w.read(reinterpret_cast<char*>(&buf), sizeof(T));
            ret[i*channel_length + ch] = buf;
        }
    }
    return ret;
}
template<typename T>
void write_wav_data(ostream& w, T* data, size_t channel_length, size_t data_length) {
    for(size_t i=0; i<data_length; i++) {
        for(size_t ch=0; ch<channel_length; ch++) {
            w.write(reinterpret_cast<char*>(&data[i*channel_length + ch]), sizeof(T));
        }
    }
}

typedef union {
    uint8_t*u8;
    uint16_t*u16;
    uint32_t*u32;
} wav_data;

wav_data read_wav ( string file_name, WavHeader& header )
{
    fstream w ( file_name, ios_base::in | ios_base::binary );
    w.read ( reinterpret_cast<char*> ( &header ), sizeof ( WavHeader ) );

    // "needs clean wav file"
    assert ( header.fmt_size == 16 );

    // check "riff" "WAVE" "fmt " "data"
    assert ( strncmp ( header.str_riff, "RIFF", 4 ) == 0 );
    assert ( strncmp ( header.str_wave, "WAVE", 4 ) == 0 );
    assert ( strncmp ( header.str_data, "data", 4 ) == 0 );
    assert ( strncmp ( header.str_fmt, "fmt ", 4 ) == 0 );


    size_t num_sample = header.data_size / (header.bit / 8) / header.channels;

    wav_data ret = {0};
    switch(header.bit) {
    case 8:
        ret.u8=read_wav_data<uint8_t>(w, header.channels, num_sample);
        break;
    case 16:
        ret.u16 = read_wav_data<uint16_t>(w, header.channels, num_sample);
        break;
    case 32:
        ret.u32=read_wav_data<uint32_t>(w, header.channels, num_sample);
        break;
    }
    w.close();
    return ret;
}

int write_wav ( string file_name, WavHeader& header, wav_data data)
{
    fstream w ( file_name, ios_base::out | ios_base::binary );

//     // following 4 members need to be set/
//     header.channels = header.channels;
//     header.sample_rate = header.sample_rate;
//     header.bit = header.bit;
//     header.data_size = header.data_size;
    strncpy(header.str_riff, "RIFF", 4);
    header.size = sizeof(WavHeader) - 8 + header.data_size;
    strncpy(header.str_wave, "WAVE", 4);
    strncpy(header.str_fmt, "fmt ", 4);
    header.fmt_size = 0x10;
    header.format = 0x01;
    strncpy(header.str_data, "data", 4);
    header.block_size = header.channels * header.bit/8;
    header.bytes_per_second = header.sample_rate * header.block_size;

    w.write ( reinterpret_cast<char*> ( &header ), sizeof ( WavHeader ) );

    size_t num_sample = header.data_size / (header.bit / 8) / header.channels;

    switch(header.bit) {
    case 8:
        write_wav_data<uint8_t>(w, data.u8, header.channels, num_sample);
        break;
    case 16:
        write_wav_data<uint16_t>(w, data.u16, header.channels, num_sample);
        break;
    case 32:
        write_wav_data<uint32_t>(w, data.u32, header.channels, num_sample);
        break;
    }
    w.close();
    return 0;
}

}
