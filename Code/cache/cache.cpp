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