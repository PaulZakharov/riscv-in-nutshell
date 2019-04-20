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

    std::vector<uint8> load_data();
    Addr get_start_PC() {
        std::cout << "START PC: "
                  << std::hex << entry_point
                  << std::endl << std::endl;
        return entry_point;
    }
};

#endif
