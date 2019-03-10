#include "instruction.h"
#include "../rf/rf.h"

using Format = Instruction::Format;

#define I(name) \
ISA_entry_generated_ ## name

struct ISAEntryGenerated {
    std::name;
    uint32 match;
    uint32 mask;
    Instruction::Executor function;
}

struct ISAEntry {
    ISAEntryGenerated generated_entry;
    Format format;
    // put here whatever else is needed

    bool match(uint32 raw) const { 
        return (raw & this->generated_entry.mask) == this->generated_entry.match;
    }
};



#define DECLARE_INSN(name, match, mask) \
static const ISAEntryGenerated I(name) = \
{ #name, match, mask, &Instruction::execute_ ## name};
#include <riscv.opcode.gen.h>
#undef DECLARE_INSN


static const std::vector<ISAEntry> ISA_table = {
   { I(lui),  Format::U },
   { I(srai), Format::I }.
   ...
}


ISAEntry find_entry(uint32 raw) {
    for (auto& x : ISA_table)
        if (x.match(raw))
            return x;
    
    assert(0);
}


Instruction::Instruction(uint32 bytes, Addr PC) :
    PC(PC)
{
    ISAEntry entry = find_entry(bytes);
    Decoder decoder(bytes);

    this->name  = entry.entry.name;
    this->format = entry.format;
    this->executor  = entry.generated_entry.function;
    this->imm_v = decoder.get_immediate(entry.format);

    // TODO: add field to ISA entry to check whether we need to do this
    this->rs1 = static_cast<Register>(decoder.rs1);
    this->rs2 = static_cast<Register>(decoder.rs2);
    this->rd = static_cast<Register>(decoder.rd);
}


void Instruction::execute() {
    (this->*function)();
    complete = true;
}
