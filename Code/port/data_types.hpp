#ifndef _DATATYPES_H_
#define _DATATYPES_H_

#include "../infra/common.hpp"
#include "../instruction/instruction.hpp"

//All classes will be extended for sure

class PreF
{
  private:
    Addr PC;
    bool is_invalid;

  public:
    PreF(): PC(NO_VAL32), is_invalid(true) {};  
    /*There should be normal NOP instr somewhere;
    what default constructor returns isn't actually NOP!*/
    PreF(Addr _PC): PC(_PC), is_invalid(false) {};
    PreF operator = (const PreF& other) {
        PreF ret(other.PC);
        return ret;        
    }
    void invalidate() {
      is_invalid = true;
    }
    void validate() {
      is_invalid = false;
    }
};

class FD
{
  private:
    Instruction instr;
    bool is_invalid;

  public:
    FD(): instr(NO_VAL32, NO_VAL32), is_invalid(true) {};
    FD(Instruction& _instr): is_invalid(false), instr(_instr) {};
    FD operator = (const FD& other) {
        FD ret(other.instr);
        return ret;        
    }
    void invalidate() {
      is_invalid = true;
    }
    void validate() {
      is_invalid = false;
    }
};

class DE
{
  private:
    uint32 reg1;
    uint32 reg2;
    Instruction instr;
    bool is_invalid;

  public:
    DE(): reg1(NO_VAL32), reg2(NO_VAL32), instr(NO_VAL32, NO_VAL32), is_invalid(true) {};
    DE(uint32 _reg1, uint32 _reg2, Instruction& _instr): reg1(_reg1), reg2(_reg2), instr(_instr), is_invalid(false) {};
    DE operator = (const DE& other) {
        DE ret(other.reg1, other.reg2, other.instr);
        return ret;        
    }
    void invalidate() {
      is_invalid = true;
    }
    void validate() {
      is_invalid = false;
    }
};

class EM
{
  private:
    uint32 reg;
    uint32 alu_result;
    Instruction instr;
    bool is_invalid;

  public:
    EM(): reg(NO_VAL32), alu_result(NO_VAL32), instr(NO_VAL32, NO_VAL32), is_invalid(true) {};
    EM(uint32 _reg, uint32 _alu, Instruction& _instr): reg(_reg), alu_result(_alu), instr(_instr), is_invalid(false) {};
    EM operator = (const EM& other) {
        EM ret(other.reg, other.alu_result, other.instr);
        return ret;        
    }
    void invalidate() {
      is_invalid = true;
    }
    void validate() {
      is_invalid = false;
    }
};

class MWB
{
  private:
    uint32 to_writeback;
    Instruction instr;
    bool is_invalid;

  public:
    MWB(): to_writeback(NO_VAL32), instr(NO_VAL32, NO_VAL32), is_invalid(true) {};
    MWB(uint32 _to_writeback, Instruction _instr): to_writeback(_to_writeback), instr(_instr), is_invalid(false) {};
    MWB operator = (const MWB& other) {
        MWB ret(other.to_writeback, other.instr);
        return ret;        
    }
    void invalidate() {
      is_invalid = true;
    }
    void validate() {
      is_invalid = false;
    }
};

#endif