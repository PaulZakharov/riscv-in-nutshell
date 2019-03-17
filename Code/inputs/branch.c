int main() {
    unsigned int a1 = 0b11111111000000001111111100000000;
    unsigned int a2 = 2;
    int b1 = 0b11111111000000001111111100000000;
    int b2 = 2;
    int c = 0;
    if (a1 < a2) c = 2;
    if (b1 < b2) c = 3;
    return 0;
}