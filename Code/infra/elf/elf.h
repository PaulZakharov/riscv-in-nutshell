#ifndef _ELF_H_
#define _ELF_H_

#include <gelf.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstdlib>
#include <stdio.h>
#include "memory.h"
#include <string>
#include <vector>

class Elf_loader{
    private:
        Elf* elf_inst;
        int fd;
        GElf_Ehdr ehdr;
        GElf_Phdr phdr;
        size_t phdrnum;
        size_t current_phdr;
    public:
        Elf_loader() : elf_inst(nullptr), fd(0), phdrnum(0), current_phdr(0) {}
        void init(const std::string& filename);
        void load_Data (std::vector<uint8>& buf);
        void end();
        //~Elf_loader();
};

#endif
