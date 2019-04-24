int main() {
	int a[10];
	int i=0;
	int j=0;
	for (i=0; i<10; i++) {
		a[i] = i;
	}
	for (i=0; i<40; i++) {
		for (j=0; j<10; j++) {
			a[j]++;
		}
	}
	return 0;
}
