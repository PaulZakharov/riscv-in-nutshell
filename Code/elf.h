#ifndef _ELF_H_
#define _ELF_H_

#include <gelf.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstdlib>
#include <stdio.h>
#include "memory.h"

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
        void Init(const char* filename);
        void Load_Memory (Memory& mem);
        void End();
        ~Elf_loader();
};

#endif
