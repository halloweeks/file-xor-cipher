#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define CHUNK_SIZE 4096 // 4KB
#define XOR_KEY 0x79 // Key in hexadecimal format

void xor_encrypt(const char* in_file, const char* out_file) {
    int in_fd = open(in_file, O_RDONLY);
    if (in_fd == -1) {
        perror("open");
        exit(1);
    }

    int out_fd = open(out_file, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (out_fd == -1) {
        perror("open");
        close(in_fd);
        exit(1);
    }

    unsigned char buffer[CHUNK_SIZE] = {0};
    
    ssize_t bytes_read = 0;

    while ((bytes_read = read(in_fd, buffer, CHUNK_SIZE)) > 0) {
        for (size_t i = 0; i < bytes_read; i++) {
            buffer[i] ^= XOR_KEY;
        }
        if (write(out_fd, buffer, bytes_read) != bytes_read) {
            perror("write");
            close(in_fd);
            close(out_fd);
            exit(1);
        }
    }

    if (bytes_read == -1) {
        perror("read");
        close(in_fd);
        close(out_fd);
        exit(1);
    }

    close(in_fd);
    close(out_fd);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s infile outfile\n", argv[0]);
        return 1;
    }

    xor_encrypt(argv[1], argv[2]);

    return 0;
}
