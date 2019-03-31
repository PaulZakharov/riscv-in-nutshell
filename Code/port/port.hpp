#ifndef _PORT_H_
#define _PORT_H_

#include "common.hpp"
#include "data_types.hpp"


template <class Data>
class Port {
    private:
        Data* data_in;
        Data* data_out;
        bool stall = false;
        bool flush = false;
    public:
        Port(){
            data_in = new Data;
            data_out = new Data;
        }; 
        void clock() {
            if (this->flush) {
                *data_out = Data; //contains nop
                data_out->validate();
                data_in->invalidate();
                this->flush = false;
            } else if ((this->stall)) {
                this->stall = false;
            } else {
                std::swap(data_in, data_out);
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
        void set_stall(bool _stall) {
            stall = _stall;
        }
        void set_flush(bool _flush) {
            flush = _flush;
        }
        bool get_stall() { return stall; }
        bool get_flush() { return flush; }
};
#endif