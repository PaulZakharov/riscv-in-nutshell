#include "memory.hpp"
#include "infra/elf/elf.hpp"


Memory::Memory(std::vector<uint8>& data):
    data(data)
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

void PerfMemory::clock() {
    auto& r = this->request;  // alias

    if (r.complete)
        return;

    r.cycles_left_to_complete -= 1;
    if (r.cycles_left_to_complete == 0) {
        if (r.is_read)
            r.data = this->read(r.addr, r.num_bytes);
        else
            this->write(r.data, r.addr, r.num_bytes);

        r.complete = true;
    }
}

RequestResult PerfMemory::get_request_status() {
    auto& r = this->request;  // alias
    if (r.complete)
        return RequestResult {.is_ready = true, .data = r.data};
    else
        return RequestResult {.is_ready = false, .data = NO_VAL32};
}
