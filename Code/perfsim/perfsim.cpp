#include "infra/config/config.hpp"
#include "perfsim.hpp"

namespace config {
    static         Value<uint64>      cache_ways     = { "cache_ways",     "cache ways",                4 };
    static         Value<uint64>      cache_sets     = { "cache_sets",     "cache sets",               64 };
    static         Value<uint64>      cache_line     = { "cache_line",     "cache line size in bytes", 16 };
    static         Value<uint64>      memory_latency = { "memory_latency", "memory latency in cycles",  3 };
}

PerfSim::PerfSim(std::string executable_filename)
    : loader(executable_filename)
    , memory(loader.load_data(), config::memory_latency)
    , icache(memory, config::cache_ways, config::cache_sets, config::cache_line)
    , dcache(memory, config::cache_ways, config::cache_sets, config::cache_line)
    , rf()
    , PC(loader.get_start_PC())
    , clocks(0)
    , ops(0)
{
    // setup stack
    rf.set_stack_pointer(memory.get_stack_pointer());
    rf.validate(Register::Number::s0);
    rf.validate(Register::Number::ra);
    rf.validate(Register::Number::s1);
    rf.validate(Register::Number::s2);
    rf.validate(Register::Number::s3);
    std::cout << config::cache_line << std::endl;
}

void PerfSim::step() {
    memory.clock();
    icache.clock();
    dcache.clock();

    this->writeback_stage();
    this->memory_stage();
    this->execute_stage();
    this->decode_stage();
    this->fetch_stage();

    std::cout << "STALLS: "
              << wires.FD_stage_reg_stall
              << wires.DE_stage_reg_stall
              << wires.EM_stage_reg_stall
              << std::endl;

    rf.dump();
    clocks++;
    if (ops > 0)
        std::cout << "CPI: " << clocks*1.0/ops << std::endl;
    std::cout << std::string(50, '-') << std::endl << std::endl;

    if (!wires.FD_stage_reg_stall)
        stage_registers.FETCH_DECODE.clock();

    if (!wires.DE_stage_reg_stall)
        stage_registers.DECODE_EXE.clock();

    if (!wires.EM_stage_reg_stall)
        stage_registers.EXE_MEM.clock();

    if (!false)
        stage_registers.MEM_WB.clock();
    
    wires.FD_stage_reg_stall = \
    wires.DE_stage_reg_stall = \
    wires.EM_stage_reg_stall = false;
}

void PerfSim::run(uint32 n) {
    for (uint32 i = 0; i < n; ++i)
        this->step();
}

void PerfSim::fetch_stage() {
    std::cout << "FETCH:  ";
    static uint fetch_stage_iterations_complete = 0;
    static bool awaiting_memory_request = false;
    static uint32 fetch_data = NO_VAL32;

    if (wires.FD_stage_reg_stall) {
        std::cout << "BUBBLE" << std::endl;
        stage_registers.FETCH_DECODE.write(nullptr);
        return;
    }
    
    // branch mispredctiion handling
    if (wires.memory_to_all_flush) {
        fetch_data = NO_VAL32;
        awaiting_memory_request = false;
        fetch_stage_iterations_complete = 0;
        PC = wires.memory_to_fetch_target;
        std::cout << "FLUSH, ";
    }

    std::cout << "PC: " << PC << std::endl;

    if (icache.is_busy()) {
        std::cout << "\tWAITING ICACHE" << std::endl;
        stage_registers.FETCH_DECODE.write(nullptr);
        return;
    }

    if (!awaiting_memory_request) {
        // send requests to memory
        Addr addr = PC + (fetch_stage_iterations_complete * 2);
        icache.send_read_request(addr, 2);
        awaiting_memory_request = true;
        std::cout << "\tsent request to icache" << std::endl;
    }

    auto request = icache.get_request_status();

    if (request.is_ready) {
        if (fetch_stage_iterations_complete == 0)
            fetch_data = request.data;
        else
            fetch_data = (fetch_data & 0xffff) | (request.data << 16);

        std::cout << "\tgot request from icache" << std::endl;
        
        awaiting_memory_request = false;
        fetch_stage_iterations_complete++;
    }

    bool fetch_complete = fetch_stage_iterations_complete == 2;

    if (fetch_complete) {
        fetch_stage_iterations_complete = 0;
        Instruction* data = new Instruction(fetch_data, PC);

        std::cout << "\t0x" << std::hex << data->get_PC() << ": "
                  << data->get_disasm() << " "
                  << std::endl;

        stage_registers.FETCH_DECODE.write(data);
        PC = PC + 4;
    } else {
        std::cout << "\tfetch_stage_iterations_complete: "
                  << fetch_stage_iterations_complete << std::endl;
        stage_registers.FETCH_DECODE.write(nullptr);
        return;
    }
}


void PerfSim::decode_stage() {
    std::cout << "DECODE: ";

    Instruction* data = nullptr;
    data = stage_registers.FETCH_DECODE.read();


    if (wires.DE_stage_reg_stall & (data != nullptr))
        wires.FD_stage_reg_stall = true;

    // branch mispredctiion handling
    if (wires.memory_to_all_flush) {
        stage_registers.DECODE_EXE.write(nullptr);
        std::cout << "FLUSH" << std::endl;
        if (data != nullptr) delete data;
        return;
    }
    

    if (data == nullptr) {
        stage_registers.DECODE_EXE.write(nullptr);
        std::cout << "BUBBLE" << std::endl;
        return;
    }

    std::cout << "0x" << std::hex << data->get_PC() << ": "
              << data->get_disasm() << " "
              << std::endl;

    // read RF registers mask
    uint32 decode_stage_regs = \
        (1 << static_cast<uint32>(data->get_rs1()))
      | (1 << static_cast<uint32>(data->get_rs2()));

    uint32 hazards = \
        (decode_stage_regs & wires.execute_stage_regs)
      | (decode_stage_regs & wires.memory_stage_regs);

    if ((hazards >> 1) != 0) {
        wires.FD_stage_reg_stall = true;
        stage_registers.DECODE_EXE.write(nullptr);
    } else {
        this->rf.read_sources(*data);
        stage_registers.DECODE_EXE.write(data);
    }

    std::cout << "\tRegisters read: " << data->get_rs1() << " " \
              << data->get_rs2() << std::endl;
}


void PerfSim::execute_stage() {
    std::cout << "EXE:    ";

    Instruction* data = nullptr;
    data = stage_registers.DECODE_EXE.read();

    wires.execute_stage_regs = 0;

    if (wires.EM_stage_reg_stall & (data != nullptr))
        wires.DE_stage_reg_stall = true;

    // branch mispredctiion handling
    if (wires.memory_to_all_flush) {
        stage_registers.EXE_MEM.write(nullptr);
        std::cout << "FLUSH" << std::endl;
        if (data != nullptr) delete data;
        return;
    }

    if (data == nullptr) {
        stage_registers.EXE_MEM.write(nullptr);
        std::cout << "BUBBLE" << std::endl;
        return;
    }

    // actual execution takes place here
    data->execute();
    wires.execute_stage_regs = (1 << static_cast<uint32>(data->get_rd())); 
    stage_registers.EXE_MEM.write(data);

    std::cout << "0x" << std::hex << data->get_PC() << ": "
                      << data->get_disasm() << " "
                      << std::endl;
}

void PerfSim::memory_stage() {
    std::cout << "MEM:    ";
    static uint memory_stage_iterations_complete = 0;
    static bool awaiting_memory_request = false;
    static uint32 memory_data = NO_VAL32;

    Instruction* data = nullptr;
    data = stage_registers.EXE_MEM.read();

    wires.memory_to_all_flush = false;
    wires.memory_to_fetch_target = NO_VAL32;
    wires.memory_stage_regs = 0;

    if (data == nullptr) {
        stage_registers.MEM_WB.write(nullptr);
        std::cout << "BUBBLE" << std::endl;
        return;
    }

    wires.memory_stage_regs = (1 << static_cast<uint32>(data->get_rd())); 

    // memory operations
    if (data->is_load() | data->is_store()) {
        if (dcache.is_busy()) {
            std::cout << "WAITING DCACHE" << std::endl;
            wires.EM_stage_reg_stall = true;
            stage_registers.MEM_WB.write(nullptr);
            return;
        }

        if (!awaiting_memory_request) {
            // send requests to memory
            Addr addr = data->get_memory_addr() + (memory_stage_iterations_complete * 2);
            size_t num_bytes = (data->get_memory_size() == 1) ? 1 : 2;

            if (data->is_load()) {
                std::cout << "READING at " << std::hex << addr << std::endl;
                dcache.send_read_request(addr, num_bytes);
            }

            if (data->is_store()) {
                memory_data = data->get_rs2_v();
                std::cout << "WRITING " << std::hex << memory_data << " at " << std::hex << addr << std::endl;
                if (memory_stage_iterations_complete == 0)
                    dcache.send_write_request(memory_data, addr, num_bytes);
                else
                    dcache.send_write_request(memory_data >> 16, addr, num_bytes);
            }

            awaiting_memory_request = true;
            std::cout << "\tsent request to dcache" << std::endl;
        }

        auto request = dcache.get_request_status();

        if (request.is_ready) {
            if (data->is_load()) {
                if (memory_stage_iterations_complete == 0)
                    memory_data = request.data;
                else
                    memory_data |= (request.data << 16);
            }

            awaiting_memory_request = false;
            memory_stage_iterations_complete++;
            std::cout << "GOT request from dcache" << std::endl;
        }

        bool memory_operation_complete = \
            (memory_stage_iterations_complete * 2) >= data->get_memory_size();

        if (memory_operation_complete) {
            memory_stage_iterations_complete = 0;
            data->set_rd_v(memory_data);
        } else {
            std::cout << "\tmemory_stage_iterations_complete: "
                      << memory_stage_iterations_complete << std::endl;
            wires.EM_stage_reg_stall = true;
            stage_registers.MEM_WB.write(nullptr);
            return;
        }
    } else {
        std::cout << "NOT a memory operation" << std::endl;
    }

    // jump operations
    if (data->is_jump() | data->is_branch()) {
        if (data->get_new_PC() != data->get_PC() + 4) {
            // target misprediction handling
            wires.memory_to_all_flush = true;
            wires.memory_to_fetch_target = data->get_new_PC();
        }
    }

    // pass data to writeback stage
    stage_registers.MEM_WB.write(data);

    std::cout << "\t0x" << std::hex << data->get_PC() << ": "
              << data->get_disasm() << " "
              << std::endl;

    if (wires.memory_to_all_flush)
        std::cout << "\tbranch misprediction, flush" << std::endl;
} 


void PerfSim::writeback_stage() {
    std::cout << "WB:     ";
    Instruction* data = nullptr;
    
    data = stage_registers.MEM_WB.read();

    if (data == nullptr) {
        std::cout << "BUBBLE" << std::endl;
        return;
    }

    std::cout << "0x" << std::hex << data->get_PC() << ": "
          << data->get_disasm() << " "
          << std::endl;
    this->rf.writeback(*data);
    ops++;
    delete data;
}
