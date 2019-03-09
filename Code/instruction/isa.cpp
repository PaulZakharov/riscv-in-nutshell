const Instruction::ISAEntry Instruction::isa_table[] = {
    // name        opcode  format,          pointer                      funct3 funct7 
    { "unknown",   0xFF,   Format::UNKNOWN, &Instruction::execute_unknown, 0x0,   0x0},
    // R type
    { "ADD",       ...,    FORMAT_R,        &Instruction::execute_add,     ...,   ...}, // TODO: fill
    // I type

    ... // TODO: make huge table here
};

