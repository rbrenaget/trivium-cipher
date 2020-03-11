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

#define U8TO32(p)               \
    (((uint32_t)((p)[0]) << 24) | \
     ((uint32_t)((p)[1]) << 16) | \
     ((uint32_t)((p)[2]) <<  8) | \
     ((uint32_t)((p)[3])     ))

#define ROTATE_LFSR_3(p, t)  \
    do {                     \
        ((p)[2]) = ((p)[1]); \
        ((p)[1]) = ((p)[0]); \
        ((p)[0]) = t;        \
    } while (0)

#define ROTATE_LFSR_4(p, t)  \
    do {                     \
        ((p)[3]) = ((p)[2]); \
        ((p)[2]) = ((p)[1]); \
        ((p)[1]) = ((p)[0]); \
        ((p)[0]) = t;        \
    } while (0)

#define GB(b, n) ((b >> n) & 1)


int TRIVIUM_init(TRIVIUM_ctx* ctx, const uint8_t key[], const uint8_t iv[], uint8_t keylen, uint8_t ivlen) {
    uint32_t t1, t2, t3;

    ctx->keylen = keylen;
    ctx->ivlen = ivlen;
    
    TRIVIUM_keysetup(ctx, key);
    TRIVIUM_ivsetup(ctx, iv);

    // Load key into LFSR A
    ctx->lfsr_a[0] = U8TO32(ctx->key);
    ctx->lfsr_a[1] = U8TO32(ctx->key + 4);
    ctx->lfsr_a[2] = U8TO32(ctx->key + 8) & 0xffff0000;

    // Load iv into LFSR B
    ctx->lfsr_b[0] = U8TO32(ctx->iv);
    ctx->lfsr_b[1] = U8TO32(ctx->iv + 4);
    ctx->lfsr_b[2] = U8TO32(ctx->iv + 8) & 0xffff0000;

    // Load LFSR C
    ctx->lfsr_c[0] = 0;
    ctx->lfsr_c[1] = 0;
    ctx->lfsr_c[2] = 0x7;

    // Initialization
    for (uint8_t i = 0; i < 36; i++) {
        // TODO: UPDATE + optimization
        t1 = ((ctx->lfsr_a[1] << 2) | (ctx->lfsr_a[2] >> 30)) ^ ((ctx->lfsr_a[1] << 29) | (ctx->lfsr_a[2] >> 3));
        t2 = ((ctx->lfsr_b[1] << 5) | (ctx->lfsr_b[2] >> 27)) ^ ((ctx->lfsr_b[1] << 20) | (ctx->lfsr_b[2] >> 12));
        t3 = ((ctx->lfsr_c[1] << 2) | (ctx->lfsr_c[2] >> 30)) ^ ((ctx->lfsr_c[2] << 15) | (ctx->lfsr_c[3] >> 17));

        t1 ^= 0;
        t2 ^= 0;
        t3 ^= 0;

        // Rotate LFSRs
        ROTATE_LFSR_3(ctx->lfsr_a, t3);
        ROTATE_LFSR_3(ctx->lfsr_b, t1);
        ROTATE_LFSR_4(ctx->lfsr_c, t2);
    }

    return 0;
}

void TRIVIUM_keysetup(TRIVIUM_ctx* ctx, const uint8_t key[]) {
    uint8_t i;

    for (i = 0; i < ctx->keylen; i++)
        ctx->key[i] = key[i];

    for (i = ctx->keylen; i < MAX_KEY_LEN; i++)
        ctx->key[i] = 0;
}

void TRIVIUM_ivsetup(TRIVIUM_ctx* ctx, const uint8_t iv[]) {
    uint8_t i;

    for (i = 0; i < ctx->ivlen; i++)
        ctx->iv[i] = iv[i];

    for (i = ctx->ivlen; i < MAX_IV_LEN; i++)
        ctx->iv[i] = 0;
}
