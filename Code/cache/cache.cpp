#include "cache.hpp"
#include <sstream>

LRUInfo::LRUInfo(Size num_ways, Size num_sets) :
    lru(num_sets)
{
    std::list<Way> l(num_ways);
    std::iota(l.begin(), l.end(), 0);
    std::fill(lru.begin(), lru.end(), l);
}

void LRUInfo::touch(Set set, Way way) {
    auto& list = this->lru[set];

    for (auto it = list.begin(); it != list.end(); ++it) {
        if (*it == way) {
            list.splice(list.begin(), list, it);
            return;
        }
    }
}

Way LRUInfo::get_LRU_way(Set set) {
    auto& list = lru[set];
    return list.back();
}

uint32 Cache::Line::read_bytes(Addr offset, Size num_bytes) {
    assert(offset + num_bytes <= this->data.size());

    uint32 value = 0;
    for (uint i = 0; i < num_bytes; ++i) {
        uint8 byte = this->data[offset + i];
        value |= static_cast<uint32>(byte) << (8*i);
    }
    return value;
}

void Cache::Line::write_bytes(uint32 value, Addr offset, Size num_bytes) {
    assert(offset + num_bytes <= this->data.size());

    for (uint i = 0; i < num_bytes; ++i) {
        uint8 byte = static_cast<uint8>(value >> 8*i); 
        this->data[offset + i] = byte;
    }
}

Cache::Cache(PerfMemory& memory,
             Size num_ways,
             Size num_sets,
             Size line_size_in_bytes)
    : memory(memory)
    , num_sets(num_sets)
    , line_size_in_bytes(line_size_in_bytes)
    , array(num_ways, std::vector<Line>(num_sets, Line(line_size_in_bytes)))
    , lru_info(num_ways, num_sets)
    { }

void Cache::process_hit(Way way) {
    std::cout << "\thit" << std::endl;
    auto& r = this->request;  // alias

    Set set = this->get_set(r.addr);
    Line& line = this->array[way][set];
    assert(line.is_valid);

    Addr offset = this->get_line_offset(r.addr);
    if (r.is_read) {
        r.data = line.read_bytes(offset, r.num_bytes);
    }
    else {
        line.write_bytes(r.data, offset, r.num_bytes);
        line.is_dirty = true;
    }
    r.complete = true;
    this->lru_info.touch(set, way);
}

void Cache::process_miss() {
    std::cout << "\tmiss" << std::endl;
    auto& r = this->request;  // alias

    Set set = this->get_set(r.addr);
    Way way = this->lru_info.get_LRU_way(set);
    Line& line = this->array[way][set];

    if (line.is_valid && line.is_dirty) {
        this->line_requests.push(
            LineRequest(this->get_line_addr(line.addr), set, way, false)
        );
        std::cout << "\tcreated write line request" << std::endl;
    }

    this->line_requests.push(
        LineRequest(this->get_line_addr(r.addr), set, way, true)
    );
    std::cout << "\tcreated read line request" << set << std::endl;
}

void Cache::process_line_requests() {
    if (this->line_requests.empty())
        return;

    std::cout << "\tprocessing requests" << std::endl;
    if (this->memory.is_busy())
        return;

    auto& lr = this->line_requests.front();
    Line& line = this->array[lr.way][lr.set];


    if (lr.is_read) {
        // read request is used to bring new line to cache from memory
        assert(!(line.is_valid && line.is_dirty));
        //assert(!(line.is_valid && line.addr != lr.addr));
    }
    else {
        // write request is used to store line in memory
        assert(line.is_valid);
        assert(line.is_dirty);
        assert(line.addr == lr.addr);
    }

    if (lr.awaiting_memory_request) {
        auto mr = this->memory.get_request_status();
        assert (mr.is_ready);
        
        if (lr.is_read)
            line.write_bytes(mr.data, lr.bytes_processed, 2);

        lr.awaiting_memory_request = false;
        lr.bytes_processed += 2;
        std::cout << "\tgot request from memory" << std::endl;
    }

    // all bytes are read/written, line request to memory is complete
    if (lr.bytes_processed == line.data.size()) {
        std::cout << "\tcompleted line request" << std::endl;
        if (lr.is_read) {
            line.is_valid = true;
            line.addr = lr.addr;
            line.is_dirty = false;
        }
        else {
            line.is_valid = true;
            line.is_dirty = false;
            assert(line.addr == lr.addr);
        }

        // drop currrent line request
        this->line_requests.pop();
        // check other line requests
        this->process_line_requests(); 
    }
    else if (!lr.awaiting_memory_request) {
        // send requests to memory
        if (lr.is_read)
            this->memory.send_read_request(lr.addr + lr.bytes_processed, 2);
        else
            this->memory.send_write_request(line.read_bytes(lr.bytes_processed, 2),
                                            lr.addr + lr.bytes_processed, 2);
        lr.awaiting_memory_request = true;
        std::cout << "\tsent request to memory" << std::endl;
    }
}

void Cache::process() {
    std::cout << "CACHE: " << std::endl;
    auto& r = this->request;  // alias

    assert(!r.complete);

    if (this->line_requests.empty()) {
        const auto [hit, way] = this->lookup(r.addr);
        if (hit)
            this->process_hit(way);
        else
            this->process_miss();
    }
    this->process_line_requests();
}


std::pair<bool, Way> Cache::lookup(Addr addr) {
    const auto set = this->get_set(addr);
    const auto tag = this->get_tag(addr);
    std::cout << "TAG " << tag << std::endl;
    for (uint way = 0; way < this->array.size(); ++way) {
        auto& line = this->array[way][set];
        std::cout << "addr " << line.addr << "tag " << this->get_tag(line.addr) << std::endl;
        if (this->get_tag(line.addr) == tag && line.is_valid) {
            return {true, way};
        }
    }
    return {false, NO_VAL32};
}

void Cache::send_read_request(Addr addr, Size num_bytes) {
    auto& r = this->request;  // alias

    if (!r.complete)
        throw std::invalid_argument("Cannot send second request!");
    if (num_bytes > 2)
        throw std::invalid_argument("Cache can't handle > 2 bytes per request");
    if ((addr % num_bytes) != 0) {
        std::stringstream stream;
        stream << "Unaligned cache access at addr " << std::hex << addr
               << " with num_bytes " << num_bytes;
        throw std::invalid_argument(stream.str());
    }

    r.is_read = true;
    r.complete = false;
    r.num_bytes = num_bytes;
    r.addr = addr;
    r.data = NO_VAL32;

    this->process();
    this->process_called_this_cycle = true;
}

void Cache::send_write_request(uint32 value, Addr addr, Size num_bytes) {
    auto& r = this->request;  // alias

    if (!r.complete)
        throw std::invalid_argument("Cannot send second request!");
    if (num_bytes > 2)
        throw std::invalid_argument("Cache can't handle > 2 bytes per request");
    if ((addr % num_bytes) != 0) {
        std::stringstream stream;
        stream << "Unaligned cache access at addr " << std::hex << addr
               << " with num_bytes " << num_bytes;
        throw std::invalid_argument(stream.str());
    }

    r.is_read = false;
    r.complete = false;
    r.num_bytes = num_bytes;
    r.addr = addr;
    r.data = value;

    this->process();
    this->process_called_this_cycle = true;
}

void Cache::clock() {
    auto& r = this->request;  // alias

    if (r.complete)
        return;

    if (!this->process_called_this_cycle)
        this->process();

    this->process_called_this_cycle = false;
}

Cache::RequestResult Cache::get_request_status() {
    auto& r = this->request;  // alias

    if (r.complete)
        return RequestResult {true, r.data};
    else
        return RequestResult {false, NO_VAL32};
}
