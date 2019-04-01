#include "infra/config/config.hpp"
#include "perfsim/perfsim.hpp"

namespace config {
    static RequiredValue<std::string> binary   = { "binary,b", "input binary file"             };
    static RequiredValue<uint64>      n        = { "nsteps,n", "number of instructions to run" };
}

int main(int argc, char** argv) {
    config::parse_args(argc, argv);
    PerfSim simulator(config::binary);
    simulator.run(config::n);
    return 0;
}
