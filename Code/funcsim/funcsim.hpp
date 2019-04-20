#ifndef FUNCSIM_H
#define FUNCSIM_H

#include "infra/common.hpp"
#include "rf/rf.hpp"
#include "memory/memory.hpp"
#include "infra/elf/elf.hpp"

class FuncSim {
    private:
    	ElfLoader loader;
        FuncMemory memory;
        RF rf;
        Addr PC = NO_VAL32;
    public:
        FuncSim(std::string executable_filename);
        void step();
        void run(uint32 n);
};

#endif

