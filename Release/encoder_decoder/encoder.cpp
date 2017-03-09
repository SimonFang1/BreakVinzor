#include <stdio.h>
#include <string.h>
#define SIZE 1024

void printHex(FILE *fp, int ch) {
	int hex1, hex2;
	hex1 = 0x0f & (ch >> 4);
	hex2 = 0x0f & ch;
	fprintf(fp, "%1x%1x", hex1, hex2);
}

int main(int argc, char* argv[]) {
	FILE *bin_file, *htf_file;
	char buffer[SIZE] = {0};
	unsigned read_size;
	bin_file = fopen(argv[1], "rb");
	if (!bin_file) {
		fprintf(stderr, "cannot open %s\n%s\n", argv[1], strerror(2));
		return 2; // No such file or directory
	}
	if (argc >= 3) {
		htf_file = fopen(argv[2], "w");
		if (!htf_file) {
			fprintf(stderr, "cannot open %s\n%s\n", argv[2], strerror(2));
			return 2; // No such file or directory
		}
	} else {
		htf_file = fopen("HexTransitFormat.txt", "w");
		if (!htf_file) {
			fprintf(stderr, "cannot open HexTransitFormat.txt\n%s\n", strerror(2));
			return 2; // No such file or directory
		}
	}
	while ((read_size = fread(&buffer, sizeof(char), SIZE, bin_file)) > 0) {
		for (int i = 0; i < read_size; i++) {
			printHex(htf_file, buffer[i]);
		}
	}
	fclose(bin_file);
	fclose(htf_file);
	return 0;
}
