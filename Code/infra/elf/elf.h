#ifndef _ELF_H_
#define _ELF_H_

#include <gelf.h>
#include <fcntl.h>
#include "infra/common.h"

class ElfLoader {
    private:
        Elf* elf_inst;
        int fd;
        GElf_Ehdr ehdr;
        GElf_Phdr phdr;
        size_t phdrnum;
        size_t current_phdr;
    public:
        ElfLoader(std::string filename);
        ~ElfLoader();
        void load_data(std::vector<uint8>& buf);
};

#endif
