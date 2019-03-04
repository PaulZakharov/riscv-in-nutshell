#include "infra/config/config.h"
//#include "func_sim/func_sim.h"
//#include "perf_sim.h"

namespace config {
    static RequiredValue<std::string> binary = {"binary,b", "input binary file"};
    static RequiredValue<uint64> n = {"nsteps,n", "number of instructions to run"};
    static Value<bool> disasm = {"disasm,d", false, "print disassembly"};
}

int main(int argc, char** argv) {
    config::parse_args(argc, argv);
//    PerfSim sim(config::disasm);
//    sim.run(config::filename, config::n);
    return 0;
}
