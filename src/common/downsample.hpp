#include "vm.hpp"


namespace pl_vm {
    using namespace std;

    template<typename T, size_t N>
    class downsample_unit: public hd_unit{
    public:
        weak_ptr<hd_wire<T>> input;
        weak_ptr<hd_wire<T>> output;
        optional<T> last_value;
        size_t counter;

        downsample_unit(weak_ptr<hd_wire<T>> in_w, weak_ptr<hd_wire<T>> out_w)
        :input(in_w), output(out_w){}

        void tick(int debug_level) override {
            if(++counter >= N){
                counter = 0;
                last_value = input.lock()->data;
            }
            output.lock()->data = last_value;
        }
    };
}
