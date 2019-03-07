#ifndef _MEMORY_H_
#define _MEMORY_H_

#include "../infra/common.h"

class Memory {
    private:
        std::vector<uint8> data;
        const Addr start_PC;
        // control information such as
        // out of range or misalign exception
        uint8 status = NO_VAL8;
    public:
        Memory(const std::string& executable_filename);

        Addr get_start_PC() const;

        uint8 load_byte(Addr address) const;
        uint16 load_half(Addr address) const;
        uint32 load_word(Addr address) const;

        void store_byte(Addr address, uint8 stored_data);
        void store_half(Addr address, uint16 stored_data);
        void store_word(Addr address, uint32 stored_data);
};

#endif
