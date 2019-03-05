#ifndef FUNCSIM_H
#define FUNCSIM_H

class FuncSim {
    private:
        RF rf;
        Memory memory;
        Addr PC = NO_VAL32;
    public:
        FuncSim(const std::string& executable_filename);
        void step();
        void run(const uint32 n); 
};

#endif

