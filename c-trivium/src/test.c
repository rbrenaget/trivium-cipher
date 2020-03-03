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
    const uint8_t key[10] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    int res;

    res = TRIVIUM_init(&ctx, key, sizeof(key), 10);

    printf("%d\n", sizeof(key));

    if (res == 0) {
        printf("Succes !\n");
    }
    else {
        printf("Something wrong !\n");
    }

    return 0;
}