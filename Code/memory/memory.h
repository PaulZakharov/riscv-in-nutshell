#ifndef _MEMORY_H_
#define _MEMORY_H_

#include <vector>

#include "infra/elf/elf.h"
#include "infra/types.h"

class Memory {
    private:
        std::vector<uint8> data;
        const Addr start_PC;

        // control information such as
        // out of range or misalign exception
        uint8 status = NO_VAL8;
    public:
        Memory(const std::string& executable_filename);

        Addr start_PC();

        uint8 load_byte(Addr address);
        uint16 load_half(Addr address);
        uint32 load_word(Addr address);

        void store_byte(Addr address, uint8 data);
        void store_half(Addr address, uint16 data);
        void store_word(Addr address, uint32 data);
};

#endif
