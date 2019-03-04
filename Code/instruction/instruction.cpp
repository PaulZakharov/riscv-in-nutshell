const Instruction::ISAEntry Instruction::isa_table[] = {
    // name        opcode  format,          pointer                      funct3 funct7 
    { "unknown",   0xFF,   Format::UNKNOWN, &Instruction::execute_unknown, 0x0,   0x0},
    // R type
    { "add",       ...,    FORMAT_R,        &Instruction::execute_add,     ...,   ...}, // TODO: fill
    // I type

    ... // TODO: make huge table here
};


Instruction::Instruction(uint32 bytes, Addr PC):
    instr(bytes),
    PC(PC)
{
    init_format();
    switch (this->format) {
        case Format::R:
            init_R();
            break;
        case Format::I:
            init_I();
            break;
        ... // TODO: finish for other types
        case Format::UNKNOWN:
            init_UNKNOWN();
            break;
    }
    new_PC = PC + 4; // to be modified in execute() for jumps
}

void Instruction::init_format() {
    for ( const auto& entry : isa_table) {
        if (match against isa entry) { // fill in the condition or create some hierarchical decoder (not recommended)
            format = ...
            name = ...
            function = ...
            break;
        }
    }
}

void Instruction::init_R() {
    // TODO: implement, set disasm
    this->rs1 = static_cast<RegNum>(instr.asR.rs1);
    this->rs2 = static_cast<RegNum>(instr.asR.rs2);
    this->rd  = static_cast<RegNum>(instr.asR.rd);
    ...
}


void Instruction::init_UNKNOWN() {
    // TODO: modify disasm
}

void Instruction::execute() {
    (this->*function)();
    complete = true;
    // TODO: modify disasm
}

