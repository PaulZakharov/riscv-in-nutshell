#include "memory.h"

void Memory::Load_byte(uint32_t address, uint32_t& reg, uint8_t reg_part) {
    if (address >= MEMORYSIZE) {
        status = status | 1u; //out of range
        return;
    }
    if (reg_part > 3) {
        status = status | 4u; //wrong part of register
        return;
    }
    uint32_t mask = ~(static_cast<uint32_t>(255) << (8*reg_part));
    reg = reg & mask | data[address]<<(8*reg_part);
    return;
}

void Memory::Load_half(uint32_t address, uint32_t& reg, uint8_t reg_part) {
    if (address >= MEMORYSIZE) {
        status = status | 1u; //out of range
        return;
    }
    if (reg_part > 1) {
        status = status | 4u; //wrong part of register
        return;
    }
    if (address % 2 != 0) {
        status = status | 2u; //misaligned load
        return;
    }
    uint32_t mask = ~(static_cast<uint32_t>(0xffff) << (16*reg_part));
    reg = reg & mask | (data[address]<<(16*reg_part)) | (data[address+1]<<(8+16*reg_part));
    return;
}

void Memory::Load_word(uint32_t address, uint32_t& reg) {
    if (address >= MEMORYSIZE) {
        status = status | 1u; //out of range
        return;
    }
    if (address % 4 != 0) {
        status = status | 2u; //misaligned load
        return;
    }
    for (int i=0; i < 4; i++) reg = reg | (data[address+i]<<(8*i));
    return;
}

void Memory::Store_byte(uint32_t address, uint32_t& reg, uint8_t reg_part){
    if (address >= MEMORYSIZE) {
        status = status | 1u; //out of range
        return;
    }
    if (reg_part > 3) {
        status = status | 4u; //wrong part of register
        return;
    }
    data[address] = (reg >> (reg_part*8)) & 0xff;
    return;
}

void Memory::Store_half(uint32_t address, uint32_t& reg, uint8_t reg_part) {
    if (address >= MEMORYSIZE) {
        status = status | 1u; //out of range
        return;
    }
    if (reg_part > 1) {
        status = status | 4u; //wrong part of register
        return;
    }
    if (address % 2 != 0) {
        status = status | 2u; //misaligned store
        return;
    }
    data[address] = (reg >> reg_part*16) & 0xff;
    data[address+1] = (reg >> (reg_part*16 + 8)) & 0xff;
    return;
}

void Memory::Store_word(uint32_t address, uint32_t& reg) {
    if (address >= MEMORYSIZE) {
        status = status | 1u; //out of range
        return;
    }
    if (address % 4 != 0) {
        status = status | 2u; //misaligned store
        return;
    }
    for (int i=0; i < 4; i++) data[address+i] = (reg >> 8*i) & 0xff;
    return;
}

int Memory::Elf_load(std::vector<uint8_t>& buf, uint32_t addr) {
    if (addr + buf.size >= MEMORYSIZE) {
        printf("Memory depleted while loading ELF\n");
        return -1;
        //exception?
    }
    for (int i=0; i<buf.size; i++) {
        data[addr+i] = buf[i];
    }
}