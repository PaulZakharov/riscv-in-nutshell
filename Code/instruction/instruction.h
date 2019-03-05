#ifndef INSTRUCTION_H
#define INSTRUCTION_H

class Instruction {
    private:
        enum class Format {
            R, I, S, B, U, J,
            UNKNOWN
        } format = Format::UNKNOWN;

        union _instr {
            struct {
                unsigned funct7  :7;
                unsigned rs2     :5;
                unsigned rs1     :5;
                unsigned funct3  :3;
                unsigned rd      :5;
                unsigned opcode  :7;
            } asR;
            struct asI {...};  // TODO: fill
            struct asS {...};  // TODO: fill
            struct asB {...};  // TODO: fill
            struct asU {...};  // TODO: fill
            struct asJ {...};  // TODO: fill
            uint32 raw {...};  // TODO: fill

            _instr(uint32 bytes) :
            raw(bytes) {}
        } instr = {};

        // TODO: create register number to ABI name mapping

        using Executor = void (Instruction::*)(void);

        struct ISAEntry // TODO: fill with needed fields
        {
            std::string name;
            uint8 opcode;
            uint8 funct3;
            uint8 funct7;
            Format format;
            Instruction::Executor function;
        };

        static const ISAEntry isa_table[];

        // registers
        Register rs1 = Register::X0;
        Register rs2 = Register::X0;
        Register rd = Register::X0;

        // values
        uint32 imm_v = NO_VAL32;
        uint32 rs1_v = NO_VAL32;
        uint32 rs2_v = NO_VAL32;
        uint32 rd_v = NO_VAL32;

        bool complete = false;
        const Addr PC = NO_VAL32;
        const Addr new_PC = NO_VAL32;

        std::string name = "INVALID"
        std::string disasm = "INVALID <args>";

        void init_format();
        void init_R();
        void init_I();
        void init_J();
        void init_UNKNOWN();

        void execute_add();
        void execute_or();
        void execute_unknown();

        // TODO: add tons of executors

        Executor function = &Instruction::execute_unknown;

    public:
        Instruction(uint32 bytes, Addr PC);
        const std::string& get_disasm() const { return disasm; }

        Register get_rs1 const { return rs1; }
        Register get_rs2 const { return rs2; }
        Register get_rd  const { return rd;  }

        bool is_jump() const;
        bool is_load() const;
        bool is_store() const;
        bool is_nop() const;

        void set_rs1_v(uint32 value);
        void set_rs2_v(uint32 value);
        void set_rd_v(uint32 value);

        uint32 get_rs1_v() const { return rs1_v;  }
        uint32 get_rs2_v() const { return rs2_v;  }
        uint32 get_rd_v()  const { return rd_v;   }

        Addr get_new_PC()  const { return new_PC; }
        Addr get_PC()      const { return PC;     }

        void execute();
};

#endif
