#include <sstream>

#include "instruction.hpp"
#include "decoder.hpp"
#include "../rf/rf.hpp"

using Format = Instruction::Format;
using Type = Instruction::Type;

// Contains fields which are generated by macro below
struct ISAEntryGenerated {
    std::string name;
    uint32 match;
    uint32 mask;
    Instruction::Executor function;
};

#define DECLARE_INSN(name, match, mask) \
static const ISAEntryGenerated ISA_entry_generated_ ## name = \
{ #name, match, mask, &Instruction::execute_ ## name};
#include "opcodes.gen.hpp"
#undef DECLARE_INSN

// contains fields which are defined in the table below
struct ISAEntry {
    ISAEntryGenerated generated_entry;
    Format format;
    size_t memory_size;
    Type type;

    bool match(uint32 raw) const { 
        return (raw & this->generated_entry.mask) == this->generated_entry.match;
    }
};


// simple macro aliases
#define I(name) \
ISA_entry_generated_ ## name

#define F(format) \
Instruction::Format::format

#define T(type) \
Instruction::Type::type

// ISA table describing instructions
static const std::vector<ISAEntry> ISA_table = {
//   name       format  memsize.    type
   { I(lui),     F(U),     0,    T(UNKNOWN) },
   { I(auipc),   F(U),     0,    T(UNKNOWN) },
   { I(jal),     F(J),     0,    T(UNKNOWN) },
   { I(jalr),    F(I),     0,    T(UNKNOWN) },
   { I(beq),     F(B),     0,    T(UNKNOWN) },
   { I(bne),     F(B),     0,    T(UNKNOWN) },
   { I(blt),     F(B),     0,    T(UNKNOWN) },
   { I(bge),     F(B),     0,    T(UNKNOWN) },
   { I(bltu),    F(B),     0,    T(UNKNOWN) },
   { I(bgeu),    F(B),     0,    T(UNKNOWN) },
   { I(lb),      F(I),     1,    T(UNKNOWN) },
   { I(lh),      F(I),     2,    T(UNKNOWN) },
   { I(lw),      F(I),     4,    T(UNKNOWN) },
   { I(lbu),     F(I),     1,    T(UNKNOWN) },
   { I(lhu),     F(I),     2,    T(UNKNOWN) },
   { I(lwu),     F(I),     4,    T(UNKNOWN) },
   { I(sb),      F(S),     1,    T(UNKNOWN) },
   { I(sh),      F(S),     2,    T(UNKNOWN) },
   { I(sw),      F(S),     4,    T(UNKNOWN) },
   { I(addi),    F(I),     0,    T(UNKNOWN) },
   { I(slti),    F(I),     0,    T(UNKNOWN) },
   { I(sltiu),   F(I),     0,    T(UNKNOWN) },
   { I(xori),    F(I),     0,    T(UNKNOWN) },
   { I(ori),     F(I),     0,    T(UNKNOWN) },
   { I(andi),    F(I),     0,    T(UNKNOWN) },
   { I(slli),    F(I),     0,    T(UNKNOWN) },
   { I(srai),    F(I),     0,    T(UNKNOWN) },
   { I(srli),    F(I),     0,    T(UNKNOWN) },
   { I(add),     F(R),     0,    T(ARITHM)  },
   { I(sub),     F(R),     0,    T(UNKNOWN) },
   { I(sll),     F(R),     0,    T(UNKNOWN) },
   { I(slt),     F(R),     0,    T(UNKNOWN) },
   { I(sltu),    F(R),     0,    T(UNKNOWN) },
   { I(xor),     F(R),     0,    T(UNKNOWN) },
   { I(or),      F(R),     0,    T(UNKNOWN) },
   { I(and),     F(R),     0,    T(UNKNOWN) },
   { I(sra),     F(R),     0,    T(UNKNOWN) },
   { I(srl),     F(R),     0,    T(UNKNOWN) }
};


const ISAEntry find_entry(uint32 raw) {
    for (const auto& x : ISA_table) {
        if (x.match(raw))
            return x;
    }
    assert(0);
}


Instruction::Instruction(uint32 bytes, Addr PC) :
    PC(PC),
    new_PC(PC + 4)
{
    ISAEntry entry = find_entry(bytes);

    this->name  = entry.generated_entry.name;
    this->format = entry.format;
    this->type = entry.type;
    this->function  = entry.generated_entry.function;
    this->memory_size = entry.memory_size;

    Decoder decoder(bytes, this->format);
    this->rs1   = decoder.get_rs1();
    this->rs2   = decoder.get_rs2();
    this->rd    = decoder.get_rd();
    this->imm_v = decoder.get_immediate();
}


std::string Instruction::get_disasm() const {
    // might pretty-format in future but
    // for debug stage this is perfect
    std::ostringstream oss;
    oss << this->name  << " ";
    oss << this->rs1   << " ";
    oss << this->rs2   << " ";
    oss << this->rd    << " ";
    oss << this->imm_v;

    return oss.str();
}


void Instruction::execute() {
    (this->*function)();
    this->complete = true;
}

