#include "perfsim.hpp"

//#define NOP_BYTES 0x13

PerfSim::PerfSim(std::string executable_filename)
    : loader(executable_filename)
    , memory(loader.load_data(), 2)
{
    // setup stack
    rf.set_stack_pointer(memory.get_stack_pointer());
    rf.validate(Register::Number::s0);
    rf.validate(Register::Number::ra);
    rf.validate(Register::Number::s1);
    rf.validate(Register::Number::s2);
    rf.validate(Register::Number::s3);

    Addr* res = new Addr(loader.get_start_PC());
    std::cout << "Start PC: " << std::hex << *res << std::endl;
    pc_stage_reg.write(res);
    pc_stage_reg.clock();
    fd_stage_reg.clock();
    de_stage_reg.clock();
    em_stage_reg.clock();
    mwb_stage_reg.clock(); 
}

void PerfSim::step() {
    
    this->writeback_stage();
    this->memory_stage();
    this->execute_stage();
    this->decode_stage();
    this->fetch_stage();

    std::cout << std::endl;
    rf.dump();
    std::cout << std::endl;
    std::cout << "Memory usage iteration: " << std::endl;
    std::cout << "Memory stage: " << wires.memory_stage_iter << ", " << \
    " Fetch stage: " << wires.fetch_stage_iter << " " << std::endl;
    std::cout << "Stalls: " << std::endl;
    std::cout << wires.PC_stage_reg_stall <<wires.FD_stage_reg_stall << \
    wires.DE_stage_reg_stall << wires.EM_stage_reg_stall << std::endl;
    std::cout << "Flushes: " << std::endl;
    std::cout << wires.PC_stage_reg_flush << wires.FD_stage_reg_flush <<\
    wires.DE_stage_reg_flush << std::endl;
    std::cout << "____________" << std::endl;
    
    wires.memory_stage_usage = false;
    if (wires.PC_stage_reg_stall) {
        wires.PC_stage_reg_stall = false;
    } else {
        pc_stage_reg.clock();
    }
    if (wires.FD_stage_reg_stall) {
        wires.FD_stage_reg_stall = false;
    } else {
        fd_stage_reg.clock();
    }
    if (wires.DE_stage_reg_stall) {
        wires.DE_stage_reg_stall = false;
    } else {
        de_stage_reg.clock();
    }
    if (wires.EM_stage_reg_stall) {
        wires.EM_stage_reg_stall = false;
    } else {
        em_stage_reg.clock();
    }
    mwb_stage_reg.clock();
    
}

void PerfSim::run(const uint32 n) {
    for (uint32 i = 0; i < n; ++i)
        this->step();
}

void PerfSim::fetch_stage() {
    std::cout << "Fetch:" << std::endl;
    Addr* data = nullptr;
    data = pc_stage_reg.read();
    if (wires.FD_stage_reg_stall & (fd_stage_reg.read() != nullptr))
        wires.PC_stage_reg_stall = true;
    if (wires.PC_stage_reg_flush) {
        wires.PC_stage_reg_flush = false;
        wires.fetch_bytes = 0;
        wires.fetch_stage_iter = 0;
        Addr* true_PC = new Addr(wires.target);
        pc_stage_reg.write(true_PC);
        fd_stage_reg.write(nullptr);
        std::cout << "NOP" << std::endl;
        if (data != nullptr) delete data;
    } else if (data != nullptr) {
        Addr PC = *data;
        std::cout << "PC: " << PC << std::endl;
        if (wires.memory_stage_usage) {
            wires.PC_stage_reg_stall = true;
            fd_stage_reg.write(nullptr);
        } else if (wires.fetch_stage_iter == 0) {
            wires.fetch_bytes = memory.read(PC, 2);
            wires.fetch_stage_iter = 1;
            wires.PC_stage_reg_stall = true;
            fd_stage_reg.write(nullptr);
        } else {
            if (!wires.PC_stage_reg_stall) {
                wires.fetch_bytes = (wires.fetch_bytes & 0xffff) | (memory.read(PC+2, 2)<<16);
                Instruction* res = new Instruction(wires.fetch_bytes, PC);
                fd_stage_reg.write(res);
                wires.fetch_stage_iter = 0;
                Addr* next_data = new Addr(PC+4);
                pc_stage_reg.write(next_data);
                std::cout << "0x" << std::hex << res->get_PC() << ": "
                    << res->get_disasm() << " "
                    << std::endl;
                delete data;
            } else {
                fd_stage_reg.write(nullptr);
            } 
        }
    }
}

void PerfSim::decode_stage() {
    std::cout << "Decode:" << std::endl;
    Instruction* data = nullptr;
    data = fd_stage_reg.read();
    if (wires.DE_stage_reg_stall & (data != nullptr))
        wires.FD_stage_reg_stall = true;
    if (wires.FD_stage_reg_flush) {
        //generate nop
        wires.FD_stage_reg_flush = false;
        de_stage_reg.write(nullptr);
        std::cout << "NOP" << std::endl;
        if (data != nullptr) delete data;
    } else if (data != nullptr) {
        std::cout << "0x" << std::hex << data->get_PC() << ": "
            << data->get_disasm() << " "
            << std::endl;
        // find out which register we have to read from
        uint32 Decode_stage_regs = ((1 << static_cast<uint32>(data->get_rs1().id())) | \
            (1 << static_cast<uint32>(data->get_rs2().id()))) >> 1;
        //stall logic due to data dependencies
        if (((Decode_stage_regs & wires.Execute_stage_regs) | \
            (Decode_stage_regs & wires.Memory_stage_regs)) != 0) {
                
            //wires.PC_stage_reg_stall = true;
            wires.FD_stage_reg_stall = true;
            de_stage_reg.write(nullptr);
        } else {
            this->rf.read_sources(*data);
            de_stage_reg.write(data);
        }
        std::cout << "Registers read: " << data->get_rs1() << " " \
          << data->get_rs2() << std::endl;
    } else {
        de_stage_reg.write(nullptr);
        std::cout << "NOP" << std::endl;
    }
    //stall logic due to stalls further in the pipeline
}

void PerfSim::execute_stage() {
    std::cout << "Execute:" << std::endl;
    Instruction* data = nullptr;
    data = de_stage_reg.read();
    if (wires.EM_stage_reg_stall & (data != nullptr))
        wires.DE_stage_reg_stall = true;
    if (wires.DE_stage_reg_flush) {
            //generate nop
            wires.DE_stage_reg_flush = false;
            em_stage_reg.write(nullptr);
            std::cout << "NOP" << std::endl;
            wires.Execute_stage_regs = 0;
            if (data!= nullptr) delete data;
    } else if (data != nullptr) {
        data->execute();
        wires.Execute_stage_regs = (1 << static_cast<uint32>(data->get_rd().id())) >> 1; 
        em_stage_reg.write(data);
        //debugging info
        std::cout << "0x" << std::hex << data->get_PC() << ": "
          << data->get_disasm() << " "
          << std::endl;
        std::cout << "Registers written to: " << data->get_rd() << std::endl;
    } else {
        em_stage_reg.write(nullptr);
        std::cout << "NOP" << std::endl;
        wires.Execute_stage_regs = 0;
    }
}

void PerfSim::memory_stage() {
    std::cout << "Memory: " << std::endl;
    Instruction* data = nullptr;
    data = em_stage_reg.read();
    if (data != nullptr) {
        wires.Memory_stage_regs = (1 << static_cast<uint32>(data->get_rd().id())) >> 1; 
        //target misprediction handling
        if (data->is_jump() | data->is_branch()) {
            if (data->get_new_PC() != data->get_PC() + 4) {
                wires.PC_stage_reg_flush = true;
                wires.FD_stage_reg_flush = true;
                wires.DE_stage_reg_flush = true;
                wires.target = data->get_new_PC();
            }
            mwb_stage_reg.write(data);
        } else if (data->is_load() | data->is_store()) {
            wires.memory_stage_usage = true;
            if (wires.memory_stage_iter == 0) {
                if (data->is_load()) {
                    uint32 value = memory.read(data->get_memory_addr(), data->get_memory_size()==1 ? 1 : 2);
                    data->set_rd_v(value);
                } else {
                    memory.write(data->get_rs2_v(), data->get_memory_addr(), data->get_memory_size()==1 ? 1 : 2);
                }
            } else {
                if (data->is_load()) {
                    uint32 value = memory.read(data->get_memory_addr()+2, data->get_memory_size()==1 ? 1 : 2);
                    data->set_rd_v(data->get_rd_v() | (value << 16));
                } else {
                    memory.write((data->get_rs2_v())>>16, data->get_memory_addr()+2, data->get_memory_size()==1 ? 1 : 2);
                }
            }
            if ((data->get_memory_size() == 4) & (wires.memory_stage_iter == 0)) {
                //wires.PC_stage_reg_stall = true;
                //wires.FD_stage_reg_stall = true;
                //wires.DE_stage_reg_stall = true;
                wires.EM_stage_reg_stall = true;
                wires.memory_stage_iter = 1;
                mwb_stage_reg.write(nullptr);
            } else {
                mwb_stage_reg.write(data);
                wires.memory_stage_iter = 0;
            }
        } else {
            mwb_stage_reg.write(data);
        }
        std::cout << "0x" << std::hex << data->get_PC() << ": "
              << data->get_disasm() << " "
              << std::endl;
        std::cout << "Registers written to: " << data->get_rd() << std::endl;
    } else {
        mwb_stage_reg.write(nullptr);
        std::cout << "NOP" << std::endl;
        wires.Memory_stage_regs = 0;
    }
} 

void PerfSim::writeback_stage() {
    std::cout << "Writeback:" << std::endl;
    Instruction* data = nullptr;
    data = mwb_stage_reg.read();
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