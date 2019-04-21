#include "memory.hpp"
#include "infra/elf/elf.hpp"


Memory::Memory(std::vector<uint8> data):
    data(std::move(data))
{ }


uint32 Memory::read(Addr addr, size_t num_bytes) const {
    assert(num_bytes <= 4);

    uint32 value = 0;
    for (uint i = 0; i < num_bytes; ++i) {
        uint8 byte = this->read_byte(addr + i);
        value |= static_cast<uint32>(byte) << (8*i);
    }

    return value;
}


void Memory::write(uint32 value, Addr addr, size_t num_bytes) {
    assert(num_bytes <= 4);

    for (uint i = 0; i < num_bytes; ++i) {
        uint8 byte = static_cast<uint8>(value >> 8*i); 
        this->write_byte(byte, addr + i);
    }
}

void PerfMemory::process() {
    auto& r = this->request;  // alias

    if (r.cycles_left_to_complete == 0) {
        if (r.is_read)
            r.data = this->read(r.addr, r.num_bytes);
        else
            this->write(r.data, r.addr, r.num_bytes);

        r.complete = true;
    }
}

void PerfMemory::send_read_request(Addr addr, size_t num_bytes) {
    auto& r = this->request;  // alias

    if (!r.complete)
        throw std::invalid_argument("Cannot send second request!");

    r.is_read = true;
    r.complete = false;
    r.cycles_left_to_complete = this->latency_in_cycles;
    r.num_bytes = num_bytes;
    r.addr = addr;

    this->process();
}

void PerfMemory::send_write_request(uint32 value, Addr addr, size_t num_bytes) {
    auto& r = this->request;  // alias

    if (!r.complete)
        throw std::invalid_argument("Cannot send second request!");

    r.is_read = false;
    r.complete = false;
    r.cycles_left_to_complete = this->latency_in_cycles;
    r.num_bytes = num_bytes;
    r.addr = addr;
    r.data = value;

    this->process();
}

void PerfMemory::clock() {
    auto& r = this->request;  // alias

    if (r.complete)
        return;

    r.cycles_left_to_complete -= 1;
    
    this->process();
}

PerfMemory::RequestResult PerfMemory::get_request_status() {
    auto& r = this->request;  // alias

    if (r.complete)
        return RequestResult {true, r.data};
    else
        return RequestResult {false, NO_VAL32};
}
