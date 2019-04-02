#ifndef _PORT_H_
#define _PORT_H_

#include "infra/common.hpp"
#include <cstring>
#include "../instruction/instruction.hpp"

#define NOP_BYTES 0x13

class PreF
{
  private:
    Addr PC;
  public:
    PreF(): PC(NO_VAL32){};  
    PreF(Addr _PC): PC(_PC){};
    Addr get_PC() { return PC; }
    void dump() {
      std::cout << "PreF PC: " << std::hex <<  PC << std::endl;
    }
};

class InstrPort
{
  private:
    Instruction instr;
  public:
    InstrPort(): instr(Instruction(NOP_BYTES, NO_VAL32)) {};
    InstrPort(Instruction _instr): instr(_instr) {};
    Instruction& get_instr(){return instr;}
    void dump() {
      std::cout << instr.get_disasm() << std::endl;
    }
};

template <class Data>
class Latch {
    private:
        Data* data_in;
        Data* data_out;
    public:
        Latch(){
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