/**
 * @file trivium.c
 * @author Romain Brenaget
 * @brief Implementation of Trivium algorithm by Christophe De Canniere and Bart Preneel
 *        Inspired from : https://www.ecrypt.eu.org/stream/e2-trivium.html
 * @version 0.1
 * @date 2020-03-03
 */

#define BLOCK_SIZE 32

#define MAX_KEY_LEN 10
#define MAX_IV_LEN 10

// Converts 4 bytes to a 32 bits unsigned integer (big endian)
#define U8TO32_BE(p)              \
    (((uint32_t)((p)[0]) << 24) | \
     ((uint32_t)((p)[1]) << 16) | \
     ((uint32_t)((p)[2]) <<  8) | \
     ((uint32_t)((p)[3])     ))

// Converts 32 bits unsigned integer to 4 bytes (little endian)
#define U32TO8_LE(p, v)            \
    (p)[0] = ((uint8_t)(v));       \
    (p)[1] = ((uint8_t)(v) >> 8);  \
    (p)[2] = ((uint8_t)(v) >> 16); \
    (p)[3] = ((uint8_t)(v) >> 24)  \

// Reverts a 32 bits unsigned integer
#define REVERT_U32(p)                       \
    (((uint32_t)((p) << 24)) |              \
     ((uint32_t)((p) <<  8) & 0x00ff0000) | \
     ((uint32_t)((p) >>  8) & 0x0000ff00) | \
     ((uint32_t)(p)  >> 24))

// One rotation of 32 bits on a LFSR of size 3 (max 96 bits)
#define ROTATE_LFSR_3(p, t) \
    ((p)[2]) = ((p)[1]);    \
    ((p)[1]) = ((p)[0]);    \
    ((p)[0]) = t            \

// One rotation of 32 bits on a LFSR of size 4 (max 128 bits)
#define ROTATE_LFSR_4(p, t) \
    ((p)[3]) = ((p)[2]);    \
    ((p)[2]) = ((p)[1]);    \
    ((p)[1]) = ((p)[0]);    \
    ((p)[0]) = t            \

// Maybe something to do better here
#define S64(p, b) (((p)[1] << ((b) - 64)) | ((p)[2] >> (96 - (b))))
#define S96(p, b) (((p)[2] << ((b) - 96)) | ((p)[3] >> (128 - (b))))

/**
 * @brief Initialization of Trivium. 
 * 
 * @param ctx Structure containing the context of Trivium.
 * @param key 10 bytes max secret key.
 * @param iv 10 bytes max initialization vector.
 * @param keylen Specifies the exact length of the key.
 * @param ivlen Specifies the exact lenght of the iv.
 */
void TRIVIUM_init(TRIVIUM_ctx* ctx, const uint8_t key[], const uint8_t iv[], uint8_t keylen, uint8_t ivlen) {
    uint32_t t1, t2, t3;

    // Store key and iv in context
    ctx->keylen = keylen;
    ctx->ivlen = ivlen;
    
    TRIVIUM_keysetup(ctx, key);
    TRIVIUM_ivsetup(ctx, iv);

    // Load key into LFSR A
    ctx->lfsr_a[0] = U8TO32_BE(ctx->key);
    ctx->lfsr_a[1] = U8TO32_BE(ctx->key + 4);
    ctx->lfsr_a[2] = U8TO32_BE(ctx->key + 8) & 0xffff0000;

    // Load iv into LFSR B
    ctx->lfsr_b[0] = U8TO32_BE(ctx->iv);
    ctx->lfsr_b[1] = U8TO32_BE(ctx->iv + 4);
    ctx->lfsr_b[2] = U8TO32_BE(ctx->iv + 8) & 0xffff0000;

    // Load LFSR C
    ctx->lfsr_c[0] = 0;
    ctx->lfsr_c[1] = 0;
    ctx->lfsr_c[2] = 0;
    ctx->lfsr_c[3] = 0x000e0000;

    // Initialization: 36 rotations of 32 bits
    for (uint8_t i = 0; i < 36; i++) {
        // Update LFSRs
        t1 = S64(ctx->lfsr_a, 66) ^ S64(ctx->lfsr_a, 93);
        t2 = S64(ctx->lfsr_b, 69) ^ S64(ctx->lfsr_b, 84);
        t3 = S64(ctx->lfsr_c, 66) ^ S96(ctx->lfsr_c, 111);

        t1 ^= (S64(ctx->lfsr_a, 91) & S64(ctx->lfsr_a, 92)) ^ S64(ctx->lfsr_b, 78);
        t2 ^= (S64(ctx->lfsr_b, 82) & S64(ctx->lfsr_b, 83)) ^ S64(ctx->lfsr_c, 87);
        t3 ^= (S96(ctx->lfsr_c, 109) & S96(ctx->lfsr_c, 110)) ^ S64(ctx->lfsr_a, 69); 

        // Rotate LFSRs
        ROTATE_LFSR_3(ctx->lfsr_a, t3);
        ROTATE_LFSR_3(ctx->lfsr_b, t1);
        ROTATE_LFSR_4(ctx->lfsr_c, t2);
    }
}

/**
 * @brief Setup the context key.
 * 
 * @param ctx Structure containing the context of Trivium.
 * @param key 10 bytes max secret key.
 */
void TRIVIUM_keysetup(TRIVIUM_ctx* ctx, const uint8_t key[]) {
    uint8_t i;

    for (i = 0; i < ctx->keylen; i++)
        ctx->key[i] = key[i];

    for (i = ctx->keylen; i < MAX_KEY_LEN; i++)
        ctx->key[i] = 0;
}

/**
 * @brief Setup the context initialization vector.
 * 
 * @param ctx Structure containing the context of Trivium.
 * @param iv 10 bytes max secret initialization vector.
 */
void TRIVIUM_ivsetup(TRIVIUM_ctx* ctx, const uint8_t iv[]) {
    uint8_t i;

    for (i = 0; i < ctx->ivlen; i++)
        ctx->iv[i] = iv[i];

    for (i = ctx->ivlen; i < MAX_IV_LEN; i++)
        ctx->iv[i] = 0;
}

/**
 * @brief Generate a 32 bits key stream by rotation. 
 * 
 * @param ctx Structure containing the context of Trivium.
 * @param output Stores the output of the key stream.
 * @param n Specifies the number of rotations.
 */
void TRIVIUM_genkeystream32(TRIVIUM_ctx* ctx, uint32_t output[], uint32_t n) {
    uint32_t t1, t2, t3;

    for (uint32_t i = 0; i < n; i++) {
        // Update LFSRs
        t1 = S64(ctx->lfsr_a, 66) ^ S64(ctx->lfsr_a, 93);
        t2 = S64(ctx->lfsr_b, 69) ^ S64(ctx->lfsr_b, 84);
        t3 = S64(ctx->lfsr_c, 66) ^ S96(ctx->lfsr_c, 111);

        output[i] = REVERT_U32(t1 ^ t2 ^ t3);       

        t1 ^= (S64(ctx->lfsr_a, 91) & S64(ctx->lfsr_a, 92)) ^ S64(ctx->lfsr_b, 78);
        t2 ^= (S64(ctx->lfsr_b, 82) & S64(ctx->lfsr_b, 83)) ^ S64(ctx->lfsr_c, 87);
        t3 ^= (S96(ctx->lfsr_c, 109) & S96(ctx->lfsr_c, 110)) ^ S64(ctx->lfsr_a, 69); 

        // Rotate LFSRs
        ROTATE_LFSR_3(ctx->lfsr_a, t3);
        ROTATE_LFSR_3(ctx->lfsr_b, t1);
        ROTATE_LFSR_4(ctx->lfsr_c, t2);
    }
}
