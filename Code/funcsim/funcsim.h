#ifndef FUNCSIM_H
#define FUNCSIM_H

#include "infra/common.h"
#include "rf/rf.h"
#include "memory/memory.h"

class FuncSim {
    private:
        Memory memory;
        RF rf;
        Addr PC = NO_VAL32;
    public:
        FuncSim(std::string executable_filename);
        void step();
        void run(uint32 n);
};

#endif

