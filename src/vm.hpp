#pragma once
#include <memory>
#include <vector>
#include "ac_datatype/ac_int.h"
#include <bitset>
#include <experimental/optional>

namespace pl_vm
{
using namespace std;
using namespace experimental;

template<typename T>
optional<string> data_signals(const optional<T>& t){
    if(t){
        return to_string(t.value());
    } else {
        return {};
    }
}

template<int W, bool S>
optional<string> data_signals(const optional<ac_int<W,S>>& t) {
    if(t){
        bitset<W> b = *(bitset<W>*)&t;
        return "b" + to_string(b);
    } else {
        return {};
    }
}

template<int W, int I, bool S, ac_q_mode Q, ac_o_mode O>
optional<string> data_signals(const optional<ac_fixed<W,I,S,Q,O>>& t){
    if(t){
        bitset<W> b = *(bitset<W>*)&t;
        return "b" + to_string(b);
    } else {
        return {};
    }
}

template<typename T>
struct data_width{
    static const size_t value = sizeof(T)*8;
};

template<int W, bool S>
struct data_width<ac_int<W,S>>{
    static const size_t value = W;
};

template<int W, int I, bool S, ac_q_mode Q, ac_o_mode O>
struct data_width<ac_fixed<W,I,S,Q,O>>{
    static const size_t value = W;
};


class hd_ticker{
public:
    virtual ~hd_ticker(){}
    virtual void tick(int debug_level)=0;
};

class hd_unit : public hd_ticker {
public:
    virtual ~hd_unit(){}
};

class hd_wire_base : public hd_ticker {
public:
    virtual ~hd_wire_base(){}
    virtual optional<string> signals(){ return {}; }
    virtual size_t width(){ return 0; }
};

template<typename T>
class hd_wire : public hd_wire_base {
public:
    typedef T Tdata;
    optional<Tdata> data;
    hd_wire():data{}{}
    virtual ~hd_wire(){}
    void tick ( int ) override {}
    optional<string> signals() override {
        return data_signals(data);
    }
    size_t width() override{
        return data_width<Tdata>::value;
    }
};


class vm {
public:
    vector<shared_ptr<hd_unit>> unit_list;
    vector<shared_ptr<hd_wire_base>> wire_list;
    vm(){}

    template<typename Tunit, typename ...Targs>
    weak_ptr<Tunit> make_unit(Targs... args){
        shared_ptr<Tunit> ret = make_shared<Tunit>(args...);
        unit_list.push_back(ret);
        return ret;
    }
    template<typename Twire, typename ...Targs>
    weak_ptr<Twire> make_wire(Targs... args){
        shared_ptr<Twire> ret = make_shared<Twire>(args...);
        wire_list.push_back(ret);
        return ret;
    }

    void tick(int debug_level) {
        for(auto iter : unit_list){
            iter.get()->tick(debug_level);
        }
        for(auto iter : wire_list){
            iter.get()->tick(debug_level);
        }
    }
};

}
