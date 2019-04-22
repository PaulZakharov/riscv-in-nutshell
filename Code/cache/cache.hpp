#include "infra/config/config.hpp"
#include "infra/common/common.h"


class Cache {
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
    };

    struct Line {
        std::vector<uint8> data;
        bool is_valid = false;
        bool is_dirty = false;
    }

    std::vector<  // way
    std::vector<  // set
        Line
    >> array;

    Line lookup(set, tag);

    Request request;
    void process();

    LRUInfo lru_info;
    
public:
    Cache(Memory& memory, uint num_ways, uint num_sets, uint line_size_in_bytes = 32);
    void clock();
    void send_read_request(Addr addr, size_t num_bytes);
    void send_write_request(uint32 value, Addr addr, size_t num_bytes);
    RequestResult get_request_status();
};
