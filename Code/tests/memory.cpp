#include "infra/test/catch.hpp"
#include "memory/memory.hpp"

//TEST_CASE("Memory read/write byte") {
//    std::vector<uint8> data {'a', 'b', 'c', 'd'};
//    Memory memory(data);
//    CHECK(memory.read_byte(1) == 'b');
//    memory.write_byte('x', 1);
//    CHECK(memory.read_byte(1) == 'x');
//}

TEST_CASE("PerfMemory read request") {
    std::vector<uint8> data {'a', 'b', 'c', 'd'};
    PerfMemory memory(data, 2);
    memory.send_read_request(1, 1);
    auto r = memory.get_request_status();
    CHECK(!r.is_ready);

    memory.clock();
    r = memory.get_request_status();
    CHECK(!r.is_ready);

    memory.clock();
    r = memory.get_request_status();
    CHECK(r.is_ready);
    CHECK(r.data == static_cast<uint32>('b'));
}

TEST_CASE("PerfMemory write request") {
    std::vector<uint8> data {'a', 'b', 'c', 'd'};
    PerfMemory memory(data, 2);
    memory.send_write_request(static_cast<uint32>('x'), 1, 1);
    auto r = memory.get_request_status();
    CHECK(!r.is_ready);

    memory.clock();
    r = memory.get_request_status();
    CHECK(!r.is_ready);

    memory.clock();
    r = memory.get_request_status();
    CHECK(r.is_ready);

    memory.clock();
    memory.send_read_request(1, 1);
    r = memory.get_request_status();

    CHECK(!r.is_ready);

    memory.clock();
    r = memory.get_request_status();
    CHECK(!r.is_ready);

    memory.clock();
    r = memory.get_request_status();
    CHECK(r.is_ready);
    CHECK(r.data == static_cast<uint32>('x'));
}
