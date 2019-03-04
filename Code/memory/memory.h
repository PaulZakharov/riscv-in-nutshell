#ifndef _MEMORY_H_
#define _MEMORY_H_

#include <vector>
#include <cstdint>
#include <unistd.h>
#include <stdio.h>

#define MEMORYSIZE 2048

class Memory{
    private:
        std::vector <uint8_t> data;
        uint8_t status; // for control information such as out of range or misalign exception
    public:
        Memory(): data(MEMORYSIZE, static_cast<uint8_t>(0)), status(0) {};
        void Load_byte(uint32_t address, uint32_t& reg, uint8_t reg_part);
        void Load_half(uint32_t address, uint32_t& reg, uint8_t reg_part);
        void Load_word(uint32_t address, uint32_t& reg);
        void Store_byte(uint32_t address, uint32_t& reg, uint8_t reg_part);
        void Store_half(uint32_t address, uint32_t& reg, uint8_t reg_part);
        void Store_word(uint32_t address, uint32_t& reg);
        int Elf_load(std::vector<uint8_t>& buf, uint32_t addr);
        // void Reset();
};


#endif