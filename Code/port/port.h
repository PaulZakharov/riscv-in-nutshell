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
        /* I don't see, why port should have distinct read and write methods; 
        this one is more hardware-like */
        void propagate(const Data& input, Data& output) {
            output = data_out;
            data_out = data_in; // rewrite via read and write
            data_in = input;    // add cycle
        };
};
#endif