#ifndef _DATATYPES_H_
#define _DATATYPES_H_

#include "../infra/common.hpp"
#include "../instruction/instruction.hpp"

//All classes will be extended for sure
#define NOP_BYTES 0x13;

class Data_ {
  private:
    bool is_valid;
    bool flush;
    bool stall;
  public:
    Data_(): is_valid(false), flush(false), stall(false) {}
    Data_(bool _valid): is_valid(_valid), flush(false), stall(false) {}
    void validate() {
        is_valid = true;
    }
    void invalidate() {
        is_valid = false;
    }
    void set_flush(bool _flush) {
        flush = _flush;
    }
    void set_stall(bool _stall) {
        stall = _stall;
    }
    bool get_flush() { return flush; }
    bool get_stall() { return stall; }
};

class PreF: public Data_
{
  private:
    Addr PC;
  public:
    PreF(): PC(NO_VAL32), Data_() {};  
    PreF(Addr _PC): PC(_PC), Data_(true) {};
    PreF operator = (const PreF& other) {
        PreF ret(other.PC);
        set_stall(other.get_stall());
        set_flush(other.get_flush());
        return ret;        
    }
};

class FD: public Data_
{
  private:
    Instruction instr;
  public:
    FD(): instr(NOP_BYTES, NO_VAL32), Data_() {};
    FD(Instruction& _instr): instr(_instr), Data_(true) {};
    FD operator = (const FD& other) {
        FD ret(other.instr);
        set_stall(other.get_stall());
        set_flush(other.get_flush());
        return ret;        
    }
};

class DE: public Data_
{
  private:
    Instruction instr;
  public:
    DE(): instr(NOP_BYTES, NO_VAL32), Data_() {};
    DE(Instruction& _instr): instr(_instr), Data_(true) {};
    DE operator = (const DE& other) {
        DE ret(other.instr);
        set_stall(other.get_stall());
        set_flush(other.get_flush());
        return ret;        
    }
};

class EM: public Data_
{
  private:
    Instruction instr;
  public:
    EM(): instr(NO_VAL32, NO_VAL32), Data_() {};
    EM(Instruction& _instr): instr(_instr), Data_(true) {};
    EM operator = (const EM& other) {
        EM ret(other.instr);
        set_stall(other.get_stall());
        set_flush(other.get_flush());
        return ret;        
    }
};

class MWB: public Data_
{
  private:
    Instruction instr;
  public:
    MWB(): instr(NOP_BYTES, NO_VAL32), Data_() {};
    MWB(uint32 _to_writeback, Instruction _instr): instr(_instr), Data_(true) {};
    MWB operator = (const MWB& other) {
        MWB ret(other.instr);
        set_stall(other.get_stall());
        set_flush(other.get_flush());
        return ret;        
    }
};

#endif