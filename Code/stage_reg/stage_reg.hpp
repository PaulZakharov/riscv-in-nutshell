#ifndef _PORT_H_
#define _PORT_H_

#include "infra/common.hpp"
#include <cstring>
#include "../instruction/instruction.hpp"

template <class Data>
class Stage_Reg {
    private:
        Data* data_in;
        Data* data_out;
    public:
        Stage_Reg(){
            data_in = nullptr;
            data_out = nullptr;
        }; 
        void clock() {
            data_out = data_in;
        }
        void write(Data* input) {
            data_in = input;
        }
        Data* read() {
            return data_out;
        }
};
#endif