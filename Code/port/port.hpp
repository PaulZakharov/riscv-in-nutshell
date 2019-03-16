#ifndef _PORT_H_
#define _PORT_H_

#include "common.hpp"
#include "data_types.hpp"

template <class Data>
class Port {
    private:
        Data data;
    public:
        Port(): data_in(), data_out() {}; 
        /* I pondered over a little bit and found out that I still don't see
        the point of doing two distinct methods. */
        void clock(const Data& input, bool stall_signal, Data& output) {
            if (stall_signal) {
                //output = stall_data;
            } else {
                output = data;
                data = input;
            }
        };
};
#endif