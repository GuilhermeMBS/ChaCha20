/**
 * @file main.c
 * @brief Example usage of the ChaCha20 library.
 *
 * This program demonstrates how to use the library to encrypt a file.
 * It reads from "tests/plaintext.bin", encrypts the data in 4KB chunks,
 * and writes the result to "tests/ciphertext.bin".
 */

#include <stdio.h>
#include "src/chacha.h"


int main(void) {
    chacha20_ctx ctx;

    uint8_t key[32] = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 
        0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 
        0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 
        0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f
    };

    uint8_t nonce[12] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4a, 
        0x00, 0x00, 0x00, 0x00
    };

    uint8_t counter[4] = {1, 0, 0, 0};

    FILE* fin = fopen("tests/ciphertext.bin", "rb");
    if (fin == NULL) {
        printf("Error: Could not open input file!\n");
        return 1;
    }

    FILE* fout = fopen("tests/plaintext.bin", "wb");
    if (fout == NULL) {
        printf("Error: Could not open output file!\n");
        fclose(fin);
        return 1;
    }

    chacha20_init(&ctx, key, counter, nonce);

    uint8_t buffer_in[4096] = { 0 };
    uint8_t buffer_out[4096] = { 0 };
    size_t bytes_read = 0;

    while ((bytes_read = fread(buffer_in, 1, 4096, fin)) > 0) {
        chacha20_update(&ctx, buffer_in, buffer_out, bytes_read);
        fwrite(buffer_out, 1, bytes_read, fout);
    }

    chacha20_wipe(&ctx);

    fclose(fin);
    fclose(fout);
    
    return 0;
}
