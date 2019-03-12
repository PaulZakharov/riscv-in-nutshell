#ifndef _MEMORY_H_
#define _MEMORY_H_

#include "../infra/common.h"

class Memory {
    private:
        std::vector<uint8> data;
        const Addr start_PC;

        void load(Instruction& instr) const {
            instr.set_v_dst(read(instr.get_memory_addr(), instr.get_mem_size()));
        }   
        void store(const Instruction& instr) {
            write(instr.get_v_src2(), instr.get_mem_addr(), instr.get_mem_size());
        }   

        uint8 load_byte(Addr address) const;
        uint16 load_half(Addr address) const;
        uint32 load_word(Addr address) const;

        void store_byte(Addr address, uint8 stored_data);
        void store_half(Addr address, uint16 stored_data);
        void store_word(Addr address, uint32 stored_data);
    public:
        Memory(const std::string& executable_filename);
        Addr get_start_PC() const;


    void load_store(FuncInstr& instr) {
        if (instr.is_load())
            load(instr);
        else if (instr.is_store())
            store(instr);
    }   

};

#endif
