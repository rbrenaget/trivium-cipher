/**
 * @file test.c
 * @author Romain Brenaget
 * @brief Simple verification test, this is not unit test
 * @version 0.1
 * @date 2020-03-03
 */

#include <stdio.h>

#include <trivium.h>


void print_byte(const uint8_t byte) {
    static const char hex[] = "0123456789ABCDEF";
    char out[3];

    out[0] = hex[(byte >> 4) & 0xf];
    out[1] = hex[byte & 0xf];
    out[2] = '\0';

    printf("%s", out);
}

void print_uint32(const uint32_t value) {
    print_byte((uint8_t)(value >> 24));
    print_byte((uint8_t)(value >> 16));
    print_byte((uint8_t)(value >> 8));
    print_byte((uint8_t)value);
}

void debug_data(const uint32_t* data, uint32_t len) {

    for (uint32_t i = 0; i < len; i++)
        print_uint32(data[i]);

    printf("\n");   
}


int main() {
    TRIVIUM_ctx ctx;
    uint32_t output[4] = {0};

    //Set 6, vector# 3:
    const uint8_t key[10] = {0xfa, 0xa7, 0x54, 0x01, 0xae, 0x5b, 0x08, 0xb5, 0x62, 0x0f};
    const uint8_t iv[10] = {0xc7, 0x60, 0xf9, 0x92, 0x2b, 0xc4, 0x5d, 0xf6, 0x8f, 0x28};

    // Set 2, vector# 0:
    //const uint8_t key[10] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    //const uint8_t iv[10] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    TRIVIUM_init(&ctx, key, iv, sizeof(key), sizeof(iv));

    printf("Keystream :\n");
    TRIVIUM_genkeystream32(&ctx, output, 4);
    debug_data(output, 4);
    print_uint32(output[0]);

    return 0;
}