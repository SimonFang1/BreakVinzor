#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define SIZE 1024

int convert(const char *a) {
    int sum = 0;
    for (int i = 0; a[i]; i++) {
        sum *= 16;
        if (isalpha(a[i])) {
            sum += a[i] - 'a' + 10;
        } else {
            sum += a[i] - '0';
        }
    }
    return sum;
}

int main(int argc, char* argv[]) {
    FILE *htf_file, *orgin_bin_file;
    char buffer[SIZE] = {0}, byte[3] = {0};
    unsigned write_size;
    orgin_bin_file = fopen(argv[1], "wb");
    if (!orgin_bin_file) {
        fprintf(stderr, "cannot open %s\n%s\n", argv[1], strerror(2));
        return 11; // Resource temporarily unavailable
    }
    if (argc >= 3) {
        htf_file = fopen(argv[2], "w");
        if (!htf_file) {
            fprintf(stderr, "cannot open %s\n%s\n", argv[2], strerror(2));
            return 2; // No such file or directory
        }
    } else {
        htf_file = fopen("HexTransitFormat.txt", "r");
        if (!htf_file) {
            fprintf(stderr, "cannot open HexTransitFormat.txt\n%s\n", strerror(2));
            return 2; // No such file or directory
        }
    }
    write_size = 0;
    byte[0] = fgetc(htf_file);
    while (!feof(htf_file)) {
        byte[1] = fgetc(htf_file);
        buffer[write_size] = convert(byte);
        write_size++;
        if (write_size == SIZE) {
            fwrite(buffer, sizeof(char), SIZE, orgin_bin_file);
            write_size = 0;
        }
        byte[0] = fgetc(htf_file);
    }
    if (write_size) {
        fwrite(buffer, sizeof(char), write_size, orgin_bin_file);
    }
    if (htf_file)
        fclose(htf_file);
    if (orgin_bin_file)
        fclose(orgin_bin_file);
    return 0;
}
