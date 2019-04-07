#ifndef MEMORY_H
#define MEMORY_H

#include "infra/common.hpp"
#include "instruction/instruction.hpp"

class Memory {
private:
    size_t size;
    std::vector<uint8> data;
    Addr start_PC;

    uint8 read_byte(Addr addr) const {
        return this->data[addr];
    }

    void write_byte(uint8 value, Addr addr) {
        this->data[addr] = value;
    }

    uint32 read(Addr addr, size_t num_bytes) const;
    void write(uint32 value, Addr addr, size_t num_bytes);

    void load_low(Instruction& instr) const {
        uint32 value = this->read(instr.get_memory_addr(), instr.get_memory_size()==1 ? 1 : 2);
        instr.set_rd_v(value);
    }

    void load_high(Instruction& instr) const {
        if (instr.get_memory_size() == 4) {
            uint32 value = this->read(instr.get_memory_addr()+2, 2);
            uint32 low = instr.get_rd_v();
            instr.set_rd_v((low & 0xffff) | (value << 16));
        }
    }

    void store_low(const Instruction& instr) {
        this->write(instr.get_rs2_v(), instr.get_memory_addr(), instr.get_memory_size()==1 ? 1 : 2);
    }
    
    void store_high(const Instruction& instr) {
        if (instr.get_memory_size() == 4) this->write(instr.get_rs2_v()>>16, instr.get_memory_addr()+2, 2);
    }     
    
public:
    Memory(std::string executable_filename);
    Addr get_start_PC() const { return start_PC; }
    Addr get_stack_pointer() const { return size - 1; }
    uint32 read_low(Addr addr) { return read(addr, 2); }
    uint32 read_high(Addr addr) { return read(addr+2, 2); }

    void load_store(Instruction& instr, int iter) {
        if (instr.is_load()){
            if (iter == 0)
                this->load_low(instr);
            else
                this->load_high(instr);
        } else if (instr.is_store()) {
            if (iter == 0)
                this->store_low(instr);
            else
                this->store_high(instr);
        }
    }
};

#endif
