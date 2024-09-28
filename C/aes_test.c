#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "aes.h"

void hex_to_bytes(const char* hex, uint8_t* bytes, size_t length) {
    for (size_t i = 0; i < length; i++) {
        sscanf(hex + 2 * i, "%2hhx", &bytes[i]);
    }
}

void print_hex(const uint8_t* data, size_t length) {
    for (size_t i = 0; i < length; i++) {
        printf("%02X", data[i]);
    }
    printf("\n");
}

int main() {
    const char* op_hex = "01020304050607080910111213140000";
    const char* k_hex = "300b000101112131415161718192a003";

    uint8_t op[16];
    uint8_t k[16];
    
    // Convert hex strings to byte arrays
    hex_to_bytes(op_hex, op, 16);
    hex_to_bytes(k_hex, k, 16);

    // Initialize AES context
    struct AES_ctx ctx;
    AES_init_ctx(&ctx, k);

    // Encrypt OP using AES in ECB mode
    uint8_t encrypted_op[16];  // Allocate memory for the encrypted output
    memcpy(encrypted_op, op, 16); // Copy OP to encrypted_op for encryption
    AES_ECB_encrypt(&ctx, encrypted_op); // Encrypt in place

    // Print results
    printf("          OP = ");
    print_hex(op, sizeof(op));
    printf("          K  = ");
    print_hex(k, sizeof(k));
    printf("encrypted_op = ");
    print_hex(encrypted_op, sizeof(encrypted_op));

    // XOR operation to calculate OPc
    uint8_t op_c[16];
    for (size_t i = 0; i < 16; i++) {
        op_c[i] = encrypted_op[i] ^ op[i];
    }
    
    printf("         OPc = ");
    print_hex(op_c, sizeof(op_c));

    return 0;
}

