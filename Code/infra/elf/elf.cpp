#include "elf.h"

void Elf_loader::init(const std::string& filename) {
    if (elf_inst || fd)
        end();
    if (elf_version(EV_CURRENT) == EV_NONE) {
        printf("ELF library init failed\n");
        exit(0);
    }
    if ((fd = open(filename.c_str(), O_RDONLY, 0)) < 0) {
        printf("File \"%s\" open failed", filename.c_str());
        exit(0);
    }
    if ((elf_inst = elf_begin(fd, ELF_C_READ, nullptr)) == nullptr) {
        printf("Elf initialization failed\n");
        exit(0);
    }
    if (elf_kind(elf_inst) != ELF_K_ELF) {
        printf("File \"%s\" is not ELF\n", filename.c_str());
        exit(0);
    }
    if (!gelf_getehdr(elf_inst, &ehdr)) {
        printf("Elf header retrieval failed\n");
        exit(0);
    }
    if (ehdr.e_machine != EM_RISCV) {
        printf("Wrong machine type\n");
        exit(0);
    }
    if (elf_getphdrnum(elf_inst, &phdrnum)) {
        printf("elf_getphdrnum failed\n");
        exit(0);
    }
    return;
}

void Elf_loader::end() {
    elf_end(elf_inst);
    elf_inst = nullptr;
    close(fd);
    fd = 0;
    return;
}

void Elf_loader::load_Data (std::vector<uint8>& buf) {
	GElf_Phdr* temp_phdr;
    long offset;
    ssize_t bytes_read;
    uint32 memory_ind = 0;
	for (int i=0; i<phdrnum; i++) {
		temp_phdr = gelf_getphdr(elf_inst, i, &phdr);
		if (temp_phdr != &phdr) {
			printf("getphdr failed\n");
			end();
			// exception?
		}
        std::vector<int8> temp_buf(phdr.p_filesz);
		if (phdr.p_type == PT_LOAD) {
            offset = lseek(fd, phdr.p_offset, SEEK_SET);
            if (offset != phdr.p_offset) {
                printf("lseek failed\n");
                end();
                //exception?
            }
            bytes_read = read(fd, &temp_buf[0], phdr.p_filesz);
            if (bytes_read != static_cast<ssize_t>(phdr.p_filesz)){
                printf("Read failed\n");
                end();
                //exception?
            }
            for (int i=0; i<phdr.p_filesz; i++) {
                buf[i+memory_ind] = temp_buf[i];
            }
            memory_ind += phdr.p_filesz;
        } else {
            continue;
        }	
    }
    return;
}




