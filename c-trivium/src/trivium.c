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

// Maybe something to do better here
#define S64(p, b) (((p)[1] << ((b) - 64)) | ((p)[2] >> (96 - (b))))
#define S96(p, b) (((p)[2] << ((b) - 96)) | ((p)[3] >> (128 - (b))))

#define GB(b, n) ((b >> n) & 1) 

void debug_data(const uint32_t* data, uint32_t len) {
    static const char hex[] = "0123456789ABCDEF";
    char out[(8*len)+1];
    uint32_t i;
    uint32_t j = 0;

    for (i = 0; j <= len; i += 8) {
        out[i] = hex[(data[j] >> 28) & 0xf];
        out[i+1] = hex[(data[j] >> 24) & 0xf];
        out[i+2] = hex[(data[j] >> 20) & 0xf];
        out[i+3] = hex[(data[j] >> 16) & 0xf];
        out[i+4] = hex[(data[j] >> 12) & 0xf];
        out[i+5] = hex[(data[j] >> 8) & 0xf];
        out[i+6] = hex[(data[j] >> 4) & 0xf];
        out[i+7] = hex[(data[j] >> 0) & 0xf];
        j++;
    }
    out[i-8] = '\0';

    printf("%s\n", out);   
}

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
    ctx->lfsr_c[2] = 0;
    ctx->lfsr_c[3] = 0x000e0000;

    debug_data(ctx->lfsr_a, 3);
    debug_data(ctx->lfsr_b, 3);
    debug_data(ctx->lfsr_c, 4);

    // Initialization
    for (uint8_t i = 0; i < 36; i++) {
        // Update LFSRs
        // TODO: optimization with #define
        /*uint32_t tx = ((ctx->lfsr_c[2] << 15) | (ctx->lfsr_c[3] >> 17));
        debug_data(&tx, 1);

        t1 = ((ctx->lfsr_a[1] << 2) | (ctx->lfsr_a[2] >> 30)) ^ ((ctx->lfsr_a[1] << 29) | (ctx->lfsr_a[2] >> 3));
        t2 = ((ctx->lfsr_b[1] << 5) | (ctx->lfsr_b[2] >> 27)) ^ ((ctx->lfsr_b[1] << 20) | (ctx->lfsr_b[2] >> 12));
        t3 = ((ctx->lfsr_c[1] << 2) | (ctx->lfsr_c[2] >> 30)) ^ ((ctx->lfsr_c[2] << 15) | (ctx->lfsr_c[3] >> 17));

        t1 ^= (((ctx->lfsr_a[1] << 27) | (ctx->lfsr_a[2] >> 5)) & ((ctx->lfsr_a[1] << 28) | (ctx->lfsr_a[2] >> 4))) ^ ((ctx->lfsr_b[1] << 14) | (ctx->lfsr_b[2] >> 18));
        t2 ^= (((ctx->lfsr_b[1] << 18) | (ctx->lfsr_b[2] >> 14)) & ((ctx->lfsr_b[1] << 19) | (ctx->lfsr_b[2] >> 13))) ^ ((ctx->lfsr_c[1] << 23) | (ctx->lfsr_c[2] >> 9));
        t3 ^= (((ctx->lfsr_c[2] << 13) | (ctx->lfsr_c[3] >> 19)) & ((ctx->lfsr_c[2] << 14) | (ctx->lfsr_c[3] >> 18))) ^ ((ctx->lfsr_a[1] << 5) | (ctx->lfsr_a[2] >> 27));*/

        t1 = S64(ctx->lfsr_a, 66) ^ S64(ctx->lfsr_a, 93);
        t2 = S64(ctx->lfsr_b, 69) ^ S64(ctx->lfsr_b, 84);
        t3 = S64(ctx->lfsr_c, 66) ^ S96(ctx->lfsr_c, 111);

        debug_data(&t3, 1);

        t1 ^= (S64(ctx->lfsr_a, 91) & S64(ctx->lfsr_a, 92)) ^ S64(ctx->lfsr_b, 78);
        t2 ^= (S64(ctx->lfsr_b, 82) & S64(ctx->lfsr_b, 83)) ^ S64(ctx->lfsr_c, 87);
        t3 ^= (S96(ctx->lfsr_c, 109) & S96(ctx->lfsr_c, 110)) ^ S64(ctx->lfsr_a, 69); 

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

void TRIVIUM_keystream(TRIVIUM_ctx* ctx, uint32_t output[], uint32_t n) {
    uint32_t t1, t2, t3;

    for (uint32_t i = 0; i < n; i++) {
        // Update LFSRs
        
        t1 = S64(ctx->lfsr_a, 66) ^ S64(ctx->lfsr_a, 93);
        t2 = S64(ctx->lfsr_b, 69) ^ S64(ctx->lfsr_b, 84);
        t3 = S64(ctx->lfsr_c, 66) ^ S96(ctx->lfsr_c, 111);

        output[i] = t1 ^ t2 ^ t3;        

        t1 ^= (S64(ctx->lfsr_a, 91) & S64(ctx->lfsr_a, 92)) ^ S64(ctx->lfsr_b, 78);
        t2 ^= (S64(ctx->lfsr_b, 82) & S64(ctx->lfsr_b, 83)) ^ S64(ctx->lfsr_c, 87);
        t3 ^= (S96(ctx->lfsr_c, 109) & S96(ctx->lfsr_c, 110)) ^ S64(ctx->lfsr_a, 69); 

        // Rotate LFSRs
        ROTATE_LFSR_3(ctx->lfsr_a, t3);
        ROTATE_LFSR_3(ctx->lfsr_b, t1);
        ROTATE_LFSR_4(ctx->lfsr_c, t2);
    }
}