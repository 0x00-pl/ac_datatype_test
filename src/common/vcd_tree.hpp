#pragma once

#include <ctime>
#include <vector>
#include <iostream>
#include <chrono>
#include "vm.hpp"

namespace pl_vm {

    using namespace std;
    using namespace chrono;
    using namespace chrono_literals;

class vcd_tree {
public:
    typedef struct {
        weak_ptr<hd_wire_base> wire;
        string name;
    } wire_ref;
    vector<wire_ref> wire_list;
    vector<shared_ptr<vcd_tree>> sub_vcd_tree;
    string name;

    vcd_tree(string name_):name(name_){}
    void registry_wire(weak_ptr<hd_wire_base> wire, string wname) {
        wire_list.push_back(wire_ref {wire, wname});
    }

    template<typename Twire, size_t N>
    void registry_wire_array(weak_ptr<Twire> (&wire)[N], string prefix="") {
        for(size_t i=0; i<N; i++){
            wire_list.push_back(wire_ref {wire[i], prefix+to_string(i)});
        }
    }
    void add_sub_tree(shared_ptr<vcd_tree> sub) {
        sub_vcd_tree.push_back(sub);
    }

    void exports_header(ostream& output, nanoseconds timescale = 1ns) {
        auto now = time(0);
        string now_str = asctime(localtime(&now));
        output << "$date " << now_str << " $end" << endl;
        output << u8"$version pl_vdc_dumper $end" << endl;
        output << "$timescale " << timescale.count() << " ns $end" << endl;
        exports_scope(output);
        output << "$enddefinitions $end" << endl;
    }

    void exports_scope(ostream& output, string prefix="") {
        output << "$scope module " << name << " $end" << endl;
        prefix = prefix + name;
        for(auto i : wire_list){
            auto width = i.wire.lock()->width();
            auto id = prefix+i.name;
            auto name = i.name;
            output << "$var wire " << width << " " << id << " " << name << " $end" << endl;
        }
        for(auto i : sub_vcd_tree){
            i->exports_scope(output, prefix + "_");
        }
        output << "$upscope $end" << endl;
    }

    void export_var_aux(ostream& output, string prefix=""){
        prefix = prefix + name;
        for(auto i : wire_list){
            auto signal = i.wire.lock()->signals();
            auto id = prefix+i.name;
            if(signal){
                auto val = signal.value();
                output << val << " " << id << endl;
            } else {
                output << "x" << " " << id << endl;
            }
        }
        for(auto i : sub_vcd_tree){
            i->export_var_aux(output, prefix + "_");
        }
    }
    void export_var(ostream& output, size_t time, string prefix=""){
        output << "#" << time << endl;
        export_var_aux(output, prefix);
    }
};

}
