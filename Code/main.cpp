#include "infra/config/config.hpp"
#include "perfsim/perfsim.hpp"
#include "funcsim/funcsim.hpp"

namespace config {
    static RequiredValue<std::string> binary   = { "binary,b",     "input binary file"             };
    static RequiredValue<uint64>      n        = { "nsteps,n",     "number of instructions to run" };
    static         Value<uint64>      func     = { "functional,f", "run in functional mode", false };
}

int main(int argc, char** argv) {
    config::parse_args(argc, argv);
    if (config::func) {
        FuncSim simulator(config::binary);
        simulator.run(config::n);
    } else {
        PerfSim simulator(config::binary);
        simulator.run(config::n);
    }
    return 0;
}
