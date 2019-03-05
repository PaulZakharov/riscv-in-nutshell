#ifndef RF_H
#define RF_H
#include "../infra/types.h"
enum class Register
{
    X0 = 0,
    X1, X2, X3, X4, X5, X6, X7, X8, X9, X10, X11, X12,
    MAX = 31
};

class RF {
private:
    
public:
    struct Reg {
        uint32 value = 0;
        bool is_valid = true; 
    } array[static_cast<uint16>(Register::MAX)];
    RF(){};
    Reg& get(Register num);
    const Reg& get_const(Register num) const;

    void invalidate(Register num);
    void validate( Register num);
    bool check( Register num) const;

    uint32 read( Register num) const;
    void write( Register num, uint32 val);

};

#endif
