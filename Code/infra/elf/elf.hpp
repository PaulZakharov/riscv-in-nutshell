#ifndef _ELF_H_
#define _ELF_H_

#include <gelf.h>
#include "infra/common.hpp"

class ElfLoader {
private:
    Elf* elf_inst;
    int fd;
    GElf_Ehdr ehdr;
    size_t phdrnum;
    Addr entry_point;
public:
    ElfLoader(std::string filename);
    ~ElfLoader();
    void load_data(std::vector<uint8>& buf);
    Addr get_start_PC() { return entry_point; }
};

#endif
