#include "perfsim.hpp"

#define NOP_BYTES 0x13

PerfSim::PerfSim(std::string executable_filename) :
    memory(executable_filename),
    rf(),
    pref_latch(),
    fd_latch(),
    de_latch(),
    em_latch(),
    mwb_latch(),
    wires()
{
    // setup stack
    rf.set_stack_pointer(memory.get_stack_pointer());
    rf.validate(Register::Number::s0);
    rf.validate(Register::Number::ra);
    rf.validate(Register::Number::s1);
    rf.validate(Register::Number::s2);
    rf.validate(Register::Number::s3);
    Addr* res = new Addr(memory.get_start_PC());
    std::cout << "Init: " << std::hex << *res << std::endl;
    pref_latch.write(res);
    pref_latch.clock();
    fd_latch.clock();
    de_latch.clock();
    em_latch.clock();
    mwb_latch.clock(); 
}

void PerfSim::step() {
    
    this->writeback_stage();
    this->memory_stage();
    this->execute_stage();
    this->decode_stage();
    this->fetch_stage();

    if (wires.PreF_latch_stall) {
        wires.PreF_latch_stall = false;
    } else {
        pref_latch.clock();
    }
    if (wires.FD_latch_stall) {
        wires.FD_latch_stall = false;
    } else {
        fd_latch.clock();
    }
    de_latch.clock();
    em_latch.clock();
    mwb_latch.clock();
    std::cout << std::endl;
    rf.dump();
    std::cout << "____________" << std::endl;
}

void PerfSim::run(const uint32 n) {
    for (uint32 i = 0; i < n; ++i)
        this->step();
}

void PerfSim::fetch_stage() {
    std::cout << "Fetch:" << std::endl;
    Addr* data = nullptr;
    data = pref_latch.read();
    if (data != nullptr) {
        Addr PC = *data;
        std::cout << "PC: " << PC << std::endl;
        if (wires.target_mispredict) {
            wires.target_mispredict = false;
            Addr* true_PC = new Addr(wires.target);
            pref_latch.write(true_PC);
            fd_latch.write(nullptr);
            std::cout << "NOP" << std::endl;
            delete data;
        } else if (!wires.PreF_latch_stall) {
            uint32 raw_bytes = memory.read_word(PC);
            Instruction* res = new Instruction(raw_bytes, PC);
            fd_latch.write(res);
            std::cout << "0x" << std::hex << res->get_PC() << ": "
              << res->get_disasm() << " "
              << std::endl;
            //PC speculation
            Addr* next_data = new Addr(PC+4);
            pref_latch.write(next_data);
            delete data;
        }
    }
}

void PerfSim::decode_stage() {
    std::cout << "Decode:" << std::endl;
    Instruction* data = nullptr;
    data = fd_latch.read();
    if (data != nullptr) {
        if (wires.FD_latch_flush) {
            //generate nop
            wires.FD_latch_flush = false;
            de_latch.write(nullptr);
            std::cout << "NOP" << std::endl;
            delete data;
        } else {
            std::cout << "0x" << std::hex << data->get_PC() << ": "
              << data->get_disasm() << " "
              << std::endl;
            // find out which register we have to read from
            uint32 Decode_stage_regs = ((1 << static_cast<uint32>(data->get_rs1().id())) | \
                (1 << static_cast<uint32>(data->get_rs2().id()))) >> 1;
            //stall logic
            if (((Decode_stage_regs & wires.Execute_stage_regs) | \
                (Decode_stage_regs & wires.Memory_stage_regs)) != 0) {
                
                wires.PreF_latch_stall = true;
                wires.FD_latch_stall = true;
                de_latch.write(nullptr);
            } else {
                this->rf.read_sources(*data);
                de_latch.write(data);
            }
            std::cout << "Registers read: " << data->get_rs1() << " " \
            << data->get_rs2() << std::endl;
        }
    } else {
        de_latch.write(nullptr);
        std::cout << "NOP" << std::endl;
    }
}

void PerfSim::execute_stage() {
    std::cout << "Execute:" << std::endl;
    Instruction* data = nullptr;
    data = de_latch.read();
    if (data != nullptr) {
        if (wires.DE_latch_flush) {
            //generate nop
            wires.DE_latch_flush = false;
            em_latch.write(nullptr);
            std::cout << "NOP" << std::endl;
            wires.Execute_stage_regs = 0;
            delete data;
        } else {
            data->execute();
            wires.Execute_stage_regs = (1 << static_cast<uint32>(data->get_rd().id())) >> 1; 
            em_latch.write(data);
            //debugging info
            std::cout << "0x" << std::hex << data->get_PC() << ": "
              << data->get_disasm() << " "
              << std::endl;
            std::cout << "Registers written to: " << data->get_rd() << std::endl;
        }
    } else {
        em_latch.write(nullptr);
        std::cout << "NOP" << std::endl;
        wires.Execute_stage_regs = 0;
    }
}

void PerfSim::memory_stage() {
    std::cout << "Memory: " << std::endl;
    Instruction* data = nullptr;
    data = em_latch.read();
    if (data != nullptr) {
        wires.Memory_stage_regs = (1 << static_cast<uint32>(data->get_rd().id())) >> 1; 
        //target misprediction handling
        if (data->is_jump() | data->is_branch()) {
            if (data->get_new_PC() != data->get_PC() + 4) {
                wires.target_mispredict = true;
                wires.FD_latch_flush = true;
                wires.DE_latch_flush = true;
                wires.target = data->get_new_PC();
            }
        }
        std::cout << "0x" << std::hex << data->get_PC() << ": "
              << data->get_disasm() << " "
              << std::endl;
        std::cout << "Registers written to: " << data->get_rd() << std::endl;
        this->memory.load_store(*data);
        mwb_latch.write(data);
    } else {
        mwb_latch.write(nullptr);
        std::cout << "NOP" << std::endl;
        wires.Memory_stage_regs = 0;
    }
} 

void PerfSim::writeback_stage() {
    std::cout << "Writeback:" << std::endl;
    Instruction* data = nullptr;
    data = mwb_latch.read();
    if (data != nullptr) {
        std::cout << "0x" << std::hex << data->get_PC() << ": "
              << data->get_disasm() << " "
              << std::endl;
        this->rf.writeback(*data);
        delete data;
    } else {
        std::cout << "NOP" << std::endl;
    }
}