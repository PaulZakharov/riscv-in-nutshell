#ifndef FUNCSIM_H
#define FUNCSIM_H

#include "infra/common.h"

class FuncSim {
    private:
        RF rf;
        Memory memory;
        Addr PC = NO_VAL32;
    public:
        FuncSim(std::string executable_filename);
        void step();
        void run(uint32 n);
};

#endif

