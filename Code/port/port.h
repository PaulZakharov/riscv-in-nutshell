#ifndef _PORT_H_
#define _PORT_H_

#include "common.h"
#include "data_types.h"

template <class Data>
class Port {
    private:
        Data data_in;
        Data data_out;
    public:
        Port(): data_in(), data_out() {}; 
        /* I pondered over a little bit and found out that I still don't see
        the point of doing two distinct methods. */
        void clock(const Data& input, Data& output) {
            output = data_out;
            data_out = data_in;
            data_in = input;
        };
};
#endif