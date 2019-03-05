#include "infra/config/config.h"
#include "funcsim/funcsim.h"
#include "perfsim/perfsim.h"

namespace config {
    static RequiredValue<std::string> binary = {"binary,b", "input binary file"};
    static RequiredValue<uint64> n = {"nsteps,n", "number of instructions to run"};
    static Value<bool> disasm = {"disasm,d", false, "print disassembly"};
}

int main(int argc, char** argv) {
    config::parse_args(argc, argv);
    FuncSim simulator(config::binary);
//    simulator.run(config::n);
    return 0;
}
