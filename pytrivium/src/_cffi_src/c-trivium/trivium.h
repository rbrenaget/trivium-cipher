/**
 * @file trivium.h
 * @author Romain Brenaget
 * @brief Header file of trivium.c
 * @version 0.1
 * @date 2020-03-03
 */

typedef struct {
    uint8_t keylen;
    uint8_t ivlen;

    uint32_t lfsr_a[3];
    uint32_t lfsr_b[3];
    uint32_t lfsr_c[4];

    uint8_t key[10];
    uint8_t iv[10];
} TRIVIUM_ctx;

extern void TRIVIUM_init(TRIVIUM_ctx* ctx, const uint8_t key[], const uint8_t iv[], uint8_t keylen, uint8_t ivlen);
extern void TRIVIUM_keysetup(TRIVIUM_ctx* ctx, const uint8_t key[]);
extern void TRIVIUM_ivsetup(TRIVIUM_ctx* ctx, const uint8_t iv[]);
extern void TRIVIUM_genkeystream32(TRIVIUM_ctx* ctx, uint32_t output[], uint32_t n);
