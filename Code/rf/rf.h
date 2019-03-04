#ifndef RF_H
#define RF_H

enum class RegNum
{
    X0 = 0,
    X1, X2, X3, X4, X5, X6, X7, X8, X9, X10, X11, X12, ...
    MAX
};

class RF {
private:
    struct Reg {
        uint32 value = 0;
        bool is_valid = true; 
    } array[RegNum::MAX];

    Reg& get(RegNum num);
    const Reg& get(RegNum num);

    void invalidate(RegNum num);
    void validate( RegNum num);
    bool check( RegNum num) const;

    uint32 read( RegNum num) const;
    void write( RegNum num, uint32 val);
public:
    RF() { }
};

#endif
