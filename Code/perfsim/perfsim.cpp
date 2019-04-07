#include "perfsim.hpp"

//#define NOP_BYTES 0x13

PerfSim::PerfSim(std::string executable_filename) :
    memory(executable_filename),
    rf(),
    pc_latch(),
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
    pc_latch.write(res);
    pc_latch.clock();
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

    std::cout << std::endl;
    rf.dump();
    std::cout << std::endl;
    std::cout << wires.memory_stage_iter << " " << \
    wires.fetch_stage_iter << " " << std::endl;
    std::cout << wires.PC_latch_stall <<wires.FD_latch_stall << \
    wires.DE_latch_stall << wires.EM_latch_stall << std::endl;
    std::cout << wires.PC_latch_flush << wires.FD_latch_flush <<\
    wires.DE_latch_flush << std::endl;
    std::cout << "____________" << std::endl;
    
    wires.memory_stage_usage = false;
    if (wires.PC_latch_stall) {
        wires.PC_latch_stall = false;
    } else {
        pc_latch.clock();
    }
    if (wires.FD_latch_stall) {
        wires.FD_latch_stall = false;
    } else {
        fd_latch.clock();
    }
    if (wires.DE_latch_stall) {
        wires.DE_latch_stall = false;
    } else {
        de_latch.clock();
    }
    if (wires.EM_latch_stall) {
        wires.EM_latch_stall = false;
    } else {
        em_latch.clock();
    }
    mwb_latch.clock();
    
}

void PerfSim::run(const uint32 n) {
    for (uint32 i = 0; i < n; ++i)
        this->step();
}

void PerfSim::fetch_stage() {
    std::cout << "Fetch:" << std::endl;
    Addr* data = nullptr;
    data = pc_latch.read();
    if (wires.DE_latch_stall & (fd_latch.read() != nullptr))
        wires.PC_latch_stall = true;
    if (wires.PC_latch_flush) {
        wires.PC_latch_flush = false;
        wires.fetch_bytes = 0;
        wires.fetch_stage_iter = 0;
        Addr* true_PC = new Addr(wires.target);
        pc_latch.write(true_PC);
        fd_latch.write(nullptr);
        std::cout << "NOP" << std::endl;
        if (data != nullptr) delete data;
    } else if (data != nullptr) {
        Addr PC = *data;
        std::cout << "PC: " << PC << std::endl;
        if (wires.memory_stage_usage) {
            wires.PC_latch_stall = true;
            fd_latch.write(nullptr);
        } else if (wires.fetch_stage_iter == 0) {
            wires.fetch_bytes = memory.read_low(PC);
            wires.fetch_stage_iter = 1;
            wires.PC_latch_stall = true;
            fd_latch.write(nullptr);
        } else {
            if (!wires.PC_latch_stall) {
                wires.fetch_bytes = (wires.fetch_bytes & 0xffff) | (memory.read_high(PC)<<16);
                Instruction* res = new Instruction(wires.fetch_bytes, PC);
                fd_latch.write(res);
                wires.fetch_stage_iter = 0;
                Addr* next_data = new Addr(PC+4);
                pc_latch.write(next_data);
                std::cout << "0x" << std::hex << res->get_PC() << ": "
                    << res->get_disasm() << " "
                    << std::endl;
                delete data;
            } else {
                fd_latch.write(nullptr);
            } 
        }
    }
}

void PerfSim::decode_stage() {
    std::cout << "Decode:" << std::endl;
    Instruction* data = nullptr;
    data = fd_latch.read();
    if (wires.DE_latch_stall & (data != nullptr))
        wires.FD_latch_stall = true;
    if (wires.FD_latch_flush) {
        //generate nop
        wires.FD_latch_flush = false;
        de_latch.write(nullptr);
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
                
            //wires.PC_latch_stall = true;
            wires.FD_latch_stall = true;
            de_latch.write(nullptr);
        } else {
            this->rf.read_sources(*data);
            de_latch.write(data);
        }
        std::cout << "Registers read: " << data->get_rs1() << " " \
          << data->get_rs2() << std::endl;
    } else {
        de_latch.write(nullptr);
        std::cout << "NOP" << std::endl;
    }
    //stall logic due to stalls further in the pipeline
}

void PerfSim::execute_stage() {
    std::cout << "Execute:" << std::endl;
    Instruction* data = nullptr;
    data = de_latch.read();
    if (wires.EM_latch_stall & (data != nullptr))
        wires.DE_latch_stall = true;
    if (wires.DE_latch_flush) {
            //generate nop
            wires.DE_latch_flush = false;
            em_latch.write(nullptr);
            std::cout << "NOP" << std::endl;
            wires.Execute_stage_regs = 0;
            if (data!= nullptr) delete data;
    } else if (data != nullptr) {
        data->execute();
        wires.Execute_stage_regs = (1 << static_cast<uint32>(data->get_rd().id())) >> 1; 
        em_latch.write(data);
        //debugging info
        std::cout << "0x" << std::hex << data->get_PC() << ": "
          << data->get_disasm() << " "
          << std::endl;
        std::cout << "Registers written to: " << data->get_rd() << std::endl;
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
                wires.PC_latch_flush = true;
                wires.FD_latch_flush = true;
                wires.DE_latch_flush = true;
                wires.target = data->get_new_PC();
            }
            mwb_latch.write(data);
        } else if (data->is_load() | data->is_store()) {
            wires.memory_stage_usage = true;
            memory.load_store(*data, wires.memory_stage_iter);
            if ((data->get_memory_size() == 4) & (wires.memory_stage_iter == 0)) {
                //wires.PC_latch_stall = true;
                //wires.FD_latch_stall = true;
                //wires.DE_latch_stall = true;
                wires.EM_latch_stall = true;
                wires.memory_stage_iter = 1;
                mwb_latch.write(nullptr);
            } else {
                mwb_latch.write(data);
                wires.memory_stage_iter = 0;
            }
        } else {
            mwb_latch.write(data);
        }
        std::cout << "0x" << std::hex << data->get_PC() << ": "
              << data->get_disasm() << " "
              << std::endl;
        std::cout << "Registers written to: " << data->get_rd() << std::endl;
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