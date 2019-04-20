#include "elf.hpp"
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <bitset>
#include <algorithm>

// TODO: replace std::cerr and exit()
// with Exceptions / use C++ ELF library

#define EM_RISCV 243

ElfLoader::ElfLoader(std::string filename) :
    elf_inst(nullptr),
    fd(0),
    phdrnum(0)
{
    if (elf_version(EV_CURRENT) == EV_NONE) {
        std::cerr << "ELF: library init failed" << std::endl;
        exit(0);
    }
    if ((fd = open(filename.c_str(), O_RDONLY, 0)) < 0) {
        std::cerr << "ELF: file " << filename << " open failed" << std::endl;
        exit(0);
    }
    if ((elf_inst = elf_begin(fd, ELF_C_READ, nullptr)) == nullptr) {
        std::cerr << "ELF: initialization failed" << std::endl;
        exit(0);
    }
    if (elf_kind(elf_inst) != ELF_K_ELF) {
        std::cerr << "ELF: file " << filename << "is not elf" << std::endl;
        exit(0);
    }
    if (!gelf_getehdr(elf_inst, &ehdr)) {
        std::cerr << "ELF: header retrieval failed" << std::endl;
        exit(0);
    }
    if (ehdr.e_machine != EM_RISCV) {
        std::cerr << "ELF: wrong machine type" << std::endl;
        exit(0);
    }
    if (elf_getphdrnum(elf_inst, &phdrnum)) {
        std::cerr << "ELF: elf_getphdrnum failed" << std::endl;
        exit(0);
    }

    this->entry_point = ehdr.e_entry;
}

ElfLoader::~ElfLoader() {
    elf_end(elf_inst);
    close(fd);
}

std::vector<uint8> ElfLoader::load_data() {
    std::vector<uint8> data;

    GElf_Phdr* temp_phdr, phdr;
    unsigned long offset = 0;
    ssize_t bytes_read = 0;
    std::bitset<8> to_out;

    for (unsigned int i = 0; i < phdrnum; i++) {
        temp_phdr = gelf_getphdr(elf_inst, i, &phdr);
        if (temp_phdr != &phdr) {
            std::cerr << "ELF: getphdr failed" << std::endl;
            exit(0);
        }

        std::vector<int8> temp_buf(phdr.p_filesz);

        if (phdr.p_type == PT_LOAD) {
            offset = lseek(fd, phdr.p_offset, SEEK_SET);
            if (offset != phdr.p_offset) {
                std::cerr << "ELF: lseek failed" << std::endl;
                exit(0);
            }

            bytes_read = read(fd, &temp_buf[0], phdr.p_filesz);
            if (bytes_read != static_cast<ssize_t>(phdr.p_filesz)){
                std::cerr << "ELF: read failed" << std::endl;
                exit(0);
            }

            if (data.size() < phdr.p_vaddr + phdr.p_filesz)
                data.resize(phdr.p_vaddr + phdr.p_filesz, 0);

            std::copy(temp_buf.begin(), temp_buf.end(),
                      data.begin() + phdr.p_vaddr);
        }
    }

    return data;
}

