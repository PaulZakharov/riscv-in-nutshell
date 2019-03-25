#ifndef _PORT_H_
#define _PORT_H_

#include "common.hpp"
#include "data_types.hpp"


template <class Data>
class Port {
    private:
        Data* data_in;
        Data* data_out;
    public:
        Port(){
            data_in = new Data();
            data_out = new Data();
        }; 
        void clock() {
            if (!data_in.stall) {
                std::swap(data_in, data_out);
                data_in->invalidate();
            } else if (data_in.flush) {
                *data_out = Data(); //contains nop
                data_out->validate();
                data_in->invalidate();
            }
        }
        void write(const Data& input) {
            *data_in = input;
            data_in->validate();
        }
        Data read() {
            output = *data_out;
            return output;
        }
};
#endif