#ifndef _PORT_H_
#define _PORT_H_

#include "infra/common.hpp"
#include "containers.hpp"


template <class Data>
class Port {
    private:
        Data& data_in;
        Data& data_out;
        bool stall = false;
        bool flush = false;
        Data dummy;
    public:
        Port(): dummy() {
            data_in = dummy;
            data_out = dummy;
        }; 
        void clock() {
            if (stall) {
                this->stall = false;
            } else {
                std::swap(data_in, data_out);
            }
            if (data_in != nullptr) {
                data_in->dump();
            } else {
                std::cout << "NULL" << std::endl;
            }
            if (data_out != nullptr) {
                data_out->dump();
            } else {
                std::cout << "NULL" << std::endl;
            }
        }
        void write(Data& input) {
            std::swap(input, data_in);
        }
        void read(Data& output) {
            std::swap(output, data_out);
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