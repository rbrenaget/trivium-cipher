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


void debug_data(const uint8_t* data, uint32_t len) {
    static const char hex[] = "0123456789abcdef";
    char out[(2*len)+1];
    uint32_t i;
    uint32_t j = 0;

    for (i = 0; j <= len; i += 2) {
        out[i] = hex[(data[j] >> 4) & 0xf];
        out[i+1] = hex[data[j] & 0xf];
        j++;
    }
    out[i-2] = '\0';

    printf("%s\n", out);   
}


int main() {
    TRIVIUM_ctx ctx;
    const uint8_t key[10] = {0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9, 0xfa};
    const uint8_t iv[10] = {0xdb, 0x92, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    int res;


    res = TRIVIUM_init(&ctx, key, iv, sizeof(key), sizeof(iv));

    if (res == 0) {
        printf("Succes !\n");
        debug_data(ctx.states, sizeof(ctx.states));
    }
    else {
        printf("Something wrong !\n");
    }

    return 0;
}