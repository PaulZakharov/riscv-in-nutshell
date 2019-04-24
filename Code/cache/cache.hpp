#ifndef CACHE_H
#define CACHE_H

#include "infra/common.hpp"
#include "memory/memory.hpp"

#include <list>
#include <queue>
#include <numeric>

using Set = uint32;
using Way = uint32;

class LRUInfo {
private:
    std::vector<  // set
        std::list<  // list of ways ordered by usage time
            Way  // way
        >
    > lru;

public:
    LRUInfo(Size ways, Size sets);

    // make given way most recently used within set
    void touch(Set set, Way way);

    // get least recently used way in a set
    Way get_LRU_way(Set set);
};

class Cache {
public:
    struct RequestResult {
        bool is_ready = false;
        uint32 data = NO_VAL32;
    };

private:
    // single cache line
    struct Line {
        std::vector<uint8> data;
        Addr addr = NO_VAL32;
        bool is_valid = false;
        bool is_dirty = false;


        Line(Size size_in_bytes) :
            data(size_in_bytes)
        { }

        uint32 read_bytes(Addr offset, Size num_bytes);
        void write_bytes(uint32 value, Addr offset, Size num_bytes);
    };

    // underlying memory (or next-level cache)
    PerfMemory& memory;
    
    // cache params
    Size num_sets;
    Size line_size_in_bytes;

    // main array of lines
    std::vector<  // way
        std::vector<  // set
            Line
        >
    > array;

    // special object to evict LRU lines
    LRUInfo lru_info;

    // read/write data request to caсhe
    struct Request {
        bool complete = true;
        bool is_read = false;
        Addr addr = NO_VAL32;
        uint32 data = NO_VAL32;
        Size num_bytes = NO_VAL32;
    };

    // line read/write request to memory
    struct LineRequest {
        bool is_read = false;
        bool awaiting_memory_request = false;
        Addr addr = NO_VAL32;
        Set set = NO_VAL32;
        Way way = NO_VAL32;
        Size bytes_processed = 0;

        LineRequest(Addr addr, Set set, Way way, bool is_read)
            : is_read(is_read)
            , addr(addr)
            , set(set)
            , way(way)
        { }
    };

    // active request to cache (single-port cache)
    Request request;

    // queue of read/write line requests to memory
    // to be processed
    std::queue<LineRequest> line_requests;

    // process active request to cache
    void process();
    bool process_called_this_cycle = false;
    void process_miss();
    void process_hit(Way way);
    void process_line_requests();

    // helper functions
    uint get_set(Addr addr) const { return (addr / line_size_in_bytes) & (num_sets - 1); }
    Addr get_tag(Addr addr) const { return (addr / line_size_in_bytes); }
    Addr get_line_addr(Addr addr) const { return addr & ~(this->line_size_in_bytes - 1); }
    Addr get_line_offset(Addr addr) const { return addr & (this->line_size_in_bytes - 1); }

    // check whether particular address is present in cache
    std::pair<bool, Way> lookup(Addr addr);

public:
    Cache(PerfMemory& memory,
          Size num_ways,
          Size num_sets,
          Size line_size_in_bytes);
    void clock();
    bool is_busy() { return !request.complete; }
    void send_read_request(Addr addr, Size num_bytes);
    void send_write_request(uint32 value, Addr addr, Size num_bytes);
    RequestResult get_request_status();
};

#endif
