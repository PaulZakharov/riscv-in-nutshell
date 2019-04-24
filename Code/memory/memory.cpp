#include "memory.hpp"
#include "infra/elf/elf.hpp"


Memory::Memory(std::vector<uint8> data) :
    data(std::move(data))
{ 
    this->data.resize(100000, 0);
}


uint32 Memory::read(Addr addr, size_t num_bytes) const {
    if (addr + num_bytes > this->data.size()){
        std::cout << "ADDR" << std::dec << addr+num_bytes << " " << std::endl;
        throw std::invalid_argument("Exceeded memory size");
    }
    uint32 value = 0;
    for (uint i = 0; i < num_bytes; ++i) {
        uint8 byte = this->read_byte(addr + i);
        value |= static_cast<uint32>(byte) << (8*i);
    }

    return value;
}


void Memory::write(uint32 value, Addr addr, size_t num_bytes) {
    if (addr + num_bytes > this->data.size())
        throw std::invalid_argument("Exceeded memory size");

    for (uint i = 0; i < num_bytes; ++i) {
        uint8 byte = static_cast<uint8>(value >> 8*i); 
        this->write_byte(byte, addr + i);
    }
}

void PerfMemory::process() {
    auto& r = this->request;  // alias

    assert(!r.complete);

    if (r.cycles_left_to_complete == 0) {
        if (r.is_read)
            r.data = this->read(r.addr, r.num_bytes);
        else
            this->write(r.data, r.addr, r.num_bytes);

        r.complete = true;
        this->request_result.is_ready = true;
        this->request_result.data = r.data;
    }
}

void PerfMemory::send_read_request(Addr addr, size_t num_bytes) {
    auto& r = this->request;  // alias

    if (!r.complete)
        throw std::invalid_argument("Cannot send second request!");
    if (num_bytes > 2)
        throw std::invalid_argument("Memory can't handle > 2 bytes per request");

    r.is_read = true;
    r.complete = false;
    r.cycles_left_to_complete = this->latency_in_cycles;
    r.num_bytes = num_bytes;
    r.addr = addr;
    r.data = NO_VAL32;
}

void PerfMemory::send_write_request(uint32 value, Addr addr, size_t num_bytes) {
    auto& r = this->request;  // alias

    if (!r.complete)
        throw std::invalid_argument("Cannot send second request!");
    if (num_bytes > 2)
        throw std::invalid_argument("Memory can't handle > 2 bytes per request");

    r.is_read = false;
    r.complete = false;
    r.cycles_left_to_complete = this->latency_in_cycles;
    r.num_bytes = num_bytes;
    r.addr = addr;
    r.data = value;
}

void PerfMemory::clock() {
    this->request_result.is_ready = false;
    this->request_result.data = NO_VAL32;
    
    auto& r = this->request;  // alias

    if (r.complete) {
        assert (r.cycles_left_to_complete == 0);
        return;
    }

    assert (r.cycles_left_to_complete > 0);
    r.cycles_left_to_complete -= 1;
    
    this->process();
}