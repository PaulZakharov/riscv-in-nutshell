#ifndef _DATATYPES_H_
#define _DATATYPES_H_

#include "../infra/common.hpp"
#include "../instruction/instruction.hpp"

//All classes will be extended for sure
#define NOP_BYTES 0x13;

class PreF
{
  private:
    Addr PC;
  public:
    PreF(): PC(NO_VAL32){};  
    PreF(Addr _PC): PC(_PC){};
    PreF operator = (const PreF& other) {
        PreF ret(other.PC);
        return ret;        
    }
    Addr get_PC() {return PC;}
};

class InstrPort
{
  private:
    Instruction instr;
  public:
    InstrPort(): instr(NOP_BYTES, NO_VAL32){};
    InstrPort(Instruction& _instr): instr(_instr) {};
    InstrPort operator = (const InstrPort& other) {
        InstrPort ret(other.instr);
        return ret;        
    }
    Instruction get_instr(){return instr;}
};

/*class DE
{
  private:
    Instruction instr;
  public:
    DE(): instr(NOP_BYTES, NO_VAL32){};
    DE(Instruction& _instr): instr(_instr){};
    DE operator = (const DE& other) {
        DE ret(other.instr);
        return ret;        
    }
    Instruction get_instr(){return instr;}
};

class EM
{
  private:
    Instruction instr;
  public:
    EM(): instr(NO_VAL32, NO_VAL32){};
    EM(Instruction& _instr): instr(_instr), Data_(true) {};
    EM operator = (const EM& other) {
        EM ret(other.instr);
        return ret;        
    }
    Instruction get_instr(){return instr;}
};

class MWB
{
  private:
    Instruction instr;
  public:
    MWB(): instr(NOP_BYTES, NO_VAL32){};
    MWB(uint32 _to_writeback, Instruction _instr): instr(_instr){};
    MWB operator = (const MWB& other) {
        MWB ret(other.instr);
        return ret;        
    }
    Instruction get_instr(){return instr;}
};*/

#endif