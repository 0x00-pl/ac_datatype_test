#pragma once
#include <memory>
#include <vector>
#include <experimental/optional>

namespace pl_vm
{
using namespace std;
using namespace experimental;

class hd_ticker{
public:
    virtual ~hd_ticker(){}
    virtual void tick(int debug_level)=0;
};

class hd_unit : public hd_ticker {
public:
    virtual ~hd_unit(){}
    void tick ( int ) override = 0;
};

class hd_wire_base : public hd_ticker {
public:
    virtual ~hd_wire_base(){}
    void tick ( int ) override = 0;
};

template<typename T>
class hd_wire : public hd_wire_base {
public:
    typedef T Tdata;
    optional<Tdata> data;
    hd_wire():data{}{}
    virtual ~hd_wire(){}
    void tick ( int ) override {}
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
