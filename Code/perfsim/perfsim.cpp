#include "perfsim.hpp"

PerfSim::PerfSim(std::string executable_filename) :
    memory(executable_filename),
    rf(),
    pref_port(),
    fd_port(),
    de_port(),
    em_port(),
    mwb_port(),
    wires()
{
    // setup stack
    rf.set_stack_pointer(memory.get_stack_pointer());
    rf.validate(Register::Number::s0);
    rf.validate(Register::Number::ra);
    rf.validate(Register::Number::s1);
    rf.validate(Register::Number::s2);
    rf.validate(Register::Number::s3);
    PreF* res = new PreF(memory.get_start_PC());
    std::cout << "Init: " << std::hex << res->get_PC() << std::endl;
    pref_port.write(res);
    pref_port.clock();
    fd_port.clock();
    de_port.clock();
    em_port.clock();
    mwb_port.clock(); 
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
        pref_port.clock();
    }
    if (wires.FD_latch_stall) {
        wires.FD_latch_stall = false;
    } else {
        fd_port.clock();
    }
    de_port.clock();
    em_port.clock();
    mwb_port.clock();
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
    PreF* data = nullptr;
    data = pref_port.read();
    if (data != nullptr) {
        Addr PC = data->get_PC();
        std::cout << "PC: " << PC << std::endl;
        if (wires.target_mispredict) {
            wires.target_mispredict = false;
            PreF* true_PC = new PreF(wires.target);
            pref_port.write(true_PC);
            fd_port.write(nullptr);
            std::cout << "NOP" << std::endl;
            delete data;
        } else if (!wires.PreF_latch_stall) {
            uint32 raw_bytes = memory.read_word(PC);
            InstrPort* res = new InstrPort(Instruction(raw_bytes, PC));
            fd_port.write(res);
            std::cout << "0x" << std::hex << res->get_instr().get_PC() << ": "
              << res->get_instr().get_disasm() << " "
              << std::endl;
            //PC speculation
            PreF* next_data = new PreF(PC+4);
            pref_port.write(next_data);
            delete data;
        }
    }
}

void PerfSim::decode_stage() {
    std::cout << "Decode:" << std::endl;
    InstrPort* data = nullptr;
    data = fd_port.read();
    if (data != nullptr) {
        if (wires.FD_latch_flush) {
            //generate nop
            wires.FD_latch_flush = false;
            de_port.write(nullptr);
            std::cout << "NOP" << std::endl;
            delete data;
        } else {
            Instruction cur_instr(data->get_instr());
            std::cout << "0x" << std::hex << data->get_instr().get_PC() << ": "
              << data->get_instr().get_disasm() << " "
              << std::endl;
            // find out which register we have to read from
            uint32 Decode_stage_regs = ((1 << static_cast<uint32>(cur_instr.get_rs1().id())) | \
                (1 << static_cast<uint32>(cur_instr.get_rs2().id()))) >> 1;
            //stall logic
            if (((Decode_stage_regs & wires.Execute_stage_regs) | \
                (Decode_stage_regs & wires.Memory_stage_regs)) != 0) {
                
                wires.PreF_latch_stall = true;
                wires.FD_latch_stall = true;
                de_port.write(nullptr);
            } else {
                this->rf.read_sources(data->get_instr());
                de_port.write(data);
            }
            std::cout << "Registers: " << Decode_stage_regs << std::endl;
        }
    } else {
        de_port.write(nullptr);
        std::cout << "NOP" << std::endl;
    }
}

void PerfSim::execute_stage() {
    std::cout << "Execute:" << std::endl;
    InstrPort* data = nullptr;
    data = de_port.read();
    if (data != nullptr) {
        if (wires.DE_latch_flush) {
            //generate nop
            wires.DE_latch_flush = false;
            em_port.write(nullptr);
            std::cout << "NOP" << std::endl;
            wires.Execute_stage_regs = 0;
            delete data;
        } else {
            data->get_instr().execute();
            wires.Execute_stage_regs = (1 << static_cast<uint32>(data->get_instr().get_rd().id())) >> 1; 
            em_port.write(data);
            //debugging info
            std::cout << "0x" << std::hex << data->get_instr().get_PC() << ": "
              << data->get_instr().get_disasm() << " "
              << std::endl;
            std::cout << "Registers: " << wires.Execute_stage_regs << std::endl;
        }
    } else {
        em_port.write(nullptr);
        std::cout << "NOP" << std::endl;
        wires.Execute_stage_regs = 0;
    }
}

void PerfSim::memory_stage() {
    std::cout << "Memory: " << std::endl;
    InstrPort* data = nullptr;
    data = em_port.read();
    if (data != nullptr) {
        Instruction cur_instr = data->get_instr();
        wires.Memory_stage_regs = (1 << static_cast<uint32>(cur_instr.get_rd().id())) >> 1; 
        //target misprediction handling
        if (cur_instr.is_jump() | cur_instr.is_branch()) {
            if (cur_instr.get_new_PC() != cur_instr.get_PC() + 4) {
                wires.target_mispredict = true;
                wires.FD_latch_flush = true;
                wires.DE_latch_flush = true;
                wires.target = cur_instr.get_new_PC();
            }
        }
        std::cout << "0x" << std::hex << data->get_instr().get_PC() << ": "
              << data->get_instr().get_disasm() << " "
              << std::endl;
        std::cout << "Registers: " << wires.Memory_stage_regs << std::endl;
        this->memory.load_store(data->get_instr());
        mwb_port.write(data);
    } else {
        mwb_port.write(nullptr);
        std::cout << "NOP" << std::endl;
        wires.Memory_stage_regs = 0;
    }
}

void PerfSim::writeback_stage() {
    std::cout << "Writeback:" << std::endl;
    InstrPort* data = nullptr;
    data = mwb_port.read();
    if (data != nullptr) {
        std::cout << "0x" << std::hex << data->get_instr().get_PC() << ": "
              << data->get_instr().get_disasm() << " "
              << std::endl;
        this->rf.writeback(data->get_instr());
        delete data;
    } else {
        std::cout << "NOP" << std::endl;
    }
}