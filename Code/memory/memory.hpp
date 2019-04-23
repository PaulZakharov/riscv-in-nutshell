#ifndef MEMORY_H
#define MEMORY_H

#include "infra/common.hpp"
#include "instruction/instruction.hpp"

class Memory {
private:
    std::vector<uint8> data;

    uint8 read_byte(Addr addr) const {
        return this->data[addr];
    }

    void write_byte(uint8 value, Addr addr) {
        this->data[addr] = value;
    }
    
protected:
    uint32 read(Addr addr, size_t num_bytes) const;
    void write(uint32 value, Addr addr, size_t num_bytes);

public:
    Memory(std::vector<uint8> data);
    Addr get_stack_pointer() const { return data.size() - 1; }
};


class FuncMemory : public Memory {
private:
    void load(Instruction& instr) const {
        uint32 value = this->read(instr.get_memory_addr(), instr.get_memory_size());
        instr.set_rd_v(value);
    }

    void store(const Instruction& instr) {
        this->write(instr.get_rs2_v(), instr.get_memory_addr(), instr.get_memory_size());
    }     

public:
    FuncMemory(std::vector<uint8> data) : Memory(data) { }

    uint32 read_word(Addr addr) { return this->read(addr, 4); }
    void load_store(Instruction& instr) {
        if (instr.is_load())
            this->load(instr);
        else if (instr.is_store())
            this->store(instr);
    }
};


class PerfMemory : public Memory {
public:
    struct RequestResult {
        bool is_ready = false;
        uint32 data = NO_VAL32;
    };

private:
    struct Request {
        bool complete = true;
        bool is_read = false;
        Addr addr = NO_VAL32;
        uint32 data = NO_VAL32;
        size_t num_bytes = NO_VAL32;
        Cycles cycles_left_to_complete = 0;
    };

    Request request;
    Cycles latency_in_cycles = 0;
    void process();

public:
    PerfMemory(std::vector<uint8> data,
               Cycles latency_in_cycles)
    : Memory(data)
    , latency_in_cycles(latency_in_cycles)
    { }

    void clock();
    bool is_busy() { return !request.complete; }
    void send_read_request(Addr addr, size_t num_bytes);
    void send_write_request(uint32 value, Addr addr, size_t num_bytes);
    RequestResult get_request_status();
};

#endif
