#pragma once
#include <array>
#include <iostream>
#include "vm.hpp"

namespace pl_vm
{
using namespace std;

template<typename T>
class input_unit : public hd_unit
{
public:
    weak_ptr<hd_wire<T>> output;
    T* data;
    size_t length;
    size_t it;

    input_unit ( weak_ptr<hd_wire<T>> out_, T* data_, size_t length_ )
        :output ( out_ ),data ( data_ ),length ( length_ ),it ( 0 ) {}

    void tick ( int ) override
    {
        auto o = output.lock();
        if ( it < length ) {
            o->data = data[it++];
        } else {
            o->data = {};
        }
    }
};

template<typename T>
class print_unit : public hd_unit
{
public:
    weak_ptr<hd_wire<T>> input;
    print_unit ( weak_ptr<hd_wire<T>> in_)
        :input ( in_ ) {}
    ~print_unit(){}

    void tick ( int ) override
    {
        auto i = input.lock();
        if(i->data){
            cout<<i->data.value()<<endl;
        } else {
            cout <<'X'<<endl;
        }
    }
};
}
