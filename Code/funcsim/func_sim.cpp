void FuncSim::step() {
    // fetch
    uint32 raw_bytes = this->memory.read_word(this->PC);

    // decode
    Instruction instruction(raw_bytes, this->PC);

    // read sources
    ...

    // execute
    instruction.execute();

    // load/store
    ...

    // writeback
    ...

    // PC update
    this->PC = instruction.get_new_PC();
}
