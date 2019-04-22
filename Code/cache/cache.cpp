LRUInfo::LRUInfo(size_t num_ways, size_t num_sets) :
    lru(num_sets)
{
    std::list<uint> l(num_ways);
    std::iota(l.begin(), l.end(), 0);
    std::fill(lru.begin(), lru.end(), l);
}

void LRUInfo::touch(uint set, uint way) {
    auto& list = this->lru[set];

    for (auto it = list.begin(); it != list.end(); ++it) {
        if (*it == way) {
            list.splice(list.begin(), list, it);
            return;
        }
    }
}

uint LRUInfo::get_LRU_way(uint set) {
    auto& list = lru[set];
    list.splice(list.begin(), list, std::prev(list.end()));
    return list.front();
}

void Cache::send_read_request(Addr addr, size_t num_bytes) {
    auto& r = this->request;  // alias

    if (!r.complete)
        throw std::invalid_argument("Cannot send second request!");

    r.is_read = true;
    r.complete = false;
    r.num_bytes = num_bytes;
    r.addr = addr;

    this->process();
}

void Cache::send_write_request(uint32 value, Addr addr, size_t num_bytes) {
    auto& r = this->request;  // alias

    if (!r.complete)
        throw std::invalid_argument("Cannot send second request!");

    r.is_read = false;
    r.complete = false;
    r.num_bytes = num_bytes;
    r.addr = addr;
    r.data = value;

    this->process();
}

void Cache::clock() {
    auto& r = this->request;  // alias

    if (r.complete)
        return;

    this->process();
}