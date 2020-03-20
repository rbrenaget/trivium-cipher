/**
 * @file test.c
 * @author Romain Brenaget
 * @brief 
 * @version 0.1
 * @date 2020-03-03
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include <stdio.h>

#include <trivium.h>


int main() {
    TRIVIUM_ctx ctx;
    //const uint8_t key[10] = {0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9, 0xfa};
    //const uint8_t iv[10] = {0xdb, 0x92, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    const uint8_t key[2] = {0x00, 0x00};
    const uint8_t iv[2] = {0x00, 0x00};
    int res;

    res = TRIVIUM_init(&ctx, key, iv, sizeof(key), sizeof(iv));

    if (res == 0) {
        uint32_t output[4] = {0};
        printf("Debug Init :\n");
        debug_data(ctx.lfsr_a, 3);
        debug_data(ctx.lfsr_b, 3);
        debug_data(ctx.lfsr_c, 4);
        TRIVIUM_keystream(&ctx, output, 2);
        debug_data(output, 4);

    }
    else {
        printf("Something wrong !\n");
    }

    return 0;
}