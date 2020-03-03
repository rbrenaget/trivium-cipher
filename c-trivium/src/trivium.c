/**
 * @file trivium.c
 * @author Romain Brenaget
 * @brief 
 * @version 0.1
 * @date 2020-03-03
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "trivium.h"
#include <stdio.h>

#define SHIFTU8R(v, n) (v >> n)
#define SHIFTU8L(v, n) (v << n)


int TRIVIUM_init(TRIVIUM_ctx* ctx, const uint8_t* key, const uint8_t* iv, uint8_t keylen, uint8_t ivlen) {
    ctx->keylen = keylen;
    ctx->ivlen = ivlen;

    TRIVIUM_keysetup(ctx, key);
    TRIVIUM_ivsetup(ctx, iv);

    return 0;
}

void TRIVIUM_keysetup(TRIVIUM_ctx* ctx, const uint8_t* key) {
    uint8_t i;

    for (i = 0; i < ctx->keylen; i++)
        ctx->key[i] = key[i];
}

void TRIVIUM_ivsetup(TRIVIUM_ctx* ctx, const uint8_t* iv) {
    uint8_t i;

    for (i = 0; i < ctx->ivlen; i++)
        ctx->iv[i] = iv[i];
}

void TRIVIUM_initialization(TRIVIUM_ctx* ctx) {
    uint8_t i;

    for (i = 0; i < ctx->keylen; i++)
        ctx->states[i] = ctx->key[i];

    ctx->states[ctx->keylen] = 0x0;
    //ctx->states[ctx->keylen+1] = ctx->iv[0] & 0xE0;
    //ctx->states[ctx->keylen+2] = ctx->iv[0] & 0x7;

    // To be continued...

    ctx->states[35] = 0x70
}