#include "memory.h"
#include "../infra/elf/elf.h"

<<<<<<< HEAD
Memory::Memory(const std::string& executable_filename): start_PC(0), data(MAX_VAL8, 0)
=======
Memory::Memory(const std::string& executable_filename) :
    start_PC(0),
    data(MAX_VAL32, 0)
>>>>>>> 01a0349a13b7d7a6ccf32685af61ed8af3270bb9
{
    Elf_loader elf_loader;
    elf_loader.init(executable_filename);
    elf_loader.load_Data(data);
    elf_loader.end();
}

Addr Memory::get_start_PC() const {
    return start_PC;
}

<<<<<<< HEAD
uint8 Memory::load_byte(Addr address) {
    if (address >= MAX_VAL8) {
=======
uint8 Memory::load_byte(Addr address) const {
    if (address >= MAX_VAL32) {
>>>>>>> 01a0349a13b7d7a6ccf32685af61ed8af3270bb9
        status = status | 1u; //out of range
        return NO_VAL8;
    }
    return data[address];
}

<<<<<<< HEAD
uint16 Memory::load_half(Addr address) {
    if (address+1 >= MAX_VAL8) {
=======
uint16 Memory::load_half(Addr address) const {
    if (address+1 >= MAX_VAL32) {
>>>>>>> 01a0349a13b7d7a6ccf32685af61ed8af3270bb9
        status = status | 1u; //out of range
        return NO_VAL16;
    }
    if (address % 2 != 0) {
        status = status | 2u; //misaligned load
        return NO_VAL16;
    }
    return data[address] | (data[address+1]<<8);
}

<<<<<<< HEAD
uint32 Memory::load_word(Addr address) {
    if (address+3 >= MAX_VAL8) {
=======
uint32 Memory::load_word(Addr address) const {
    if (address+3 >= MAX_VAL32) {
>>>>>>> 01a0349a13b7d7a6ccf32685af61ed8af3270bb9
        status = status | 1u; //out of range
        return NO_VAL32;
    }
    if (address % 4 != 0) {
        status = status | 2u; //misaligned load
        return NO_VAL32;
    }
    return data[address] | data[address+1]<<8 | data[address+2]<<16 | data[address+3]<<24;
}

void Memory::store_byte(Addr address, uint8 stored_data){
    if (address >= MAX_VAL8) {
        status = status | 1u; //out of range
        return;
    }
    data[address] = stored_data;
}

void Memory::store_half(Addr address, uint16 stored_data){
    if (address >= MAX_VAL8) {
        status = status | 1u; //out of range
        return;
    }
    if (address % 2 != 0) {
        status = status | 2u; //misaligned store
        return;
    }
    data[address] = stored_data & 0xff;
    data[address+1] = stored_data>>8 & 0xff;
}

void Memory::store_word(Addr address, uint32 stored_data) {
    if (address >= MAX_VAL8) {
        status = status | 1u; //out of range
        return;
    }
    if (address % 4 != 0) {
        status = status | 2u; //misaligned store
        return;
    }
    for (int i=0; i < 4; i++) data[address+i] = (stored_data >> 8*i) & 0xff;
    return;
}
