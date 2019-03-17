int main () {
    int i =1424324;
    int j = 209525;
    int c = func(i, j);
    c = c + i&j;
    return 0;
}

int func (int i, int j) {
    int k = i ^ j;
    k = k << 2;
    return k;
}