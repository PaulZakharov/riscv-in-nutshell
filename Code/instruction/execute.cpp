void Instruction::execute_unknown() {
    std::cerr << "Incorrect instruction: " << this->disasm << std::endl;
    exit(EXIT_FAILURE);
}

void Instruction::execute_add() {
    this->rd_v = this->rs1_v + ... // TODO
}

void Instruction::execute_or() {
    this->rd_v = ... // TODO
}

// TODO: make a huge list of functions implementations
