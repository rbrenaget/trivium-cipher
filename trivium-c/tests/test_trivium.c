#include <assert.h>
#include <trivium.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

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

void test_set6_vector3() {
    const uint8_t key[10] = {0xfa, 0xa7, 0x54, 0x01, 0xae, 0x5b, 0x08, 0xb5, 0x62, 0x0f};
    const uint8_t iv[10] = {0xc7, 0x60, 0xf9, 0x92, 0x2b, 0xc4, 0x5d, 0xf6, 0x8f, 0x28};
    
    TRIVIUM_ctx ctx;
    TRIVIUM_init(&ctx, key, iv, 10, 10);
    
    uint32_t output[64];
    TRIVIUM_genkeystream32(&ctx, output, 64);

    debug_data(output, 1);
    
    // Verify first few output words match expected values
    assert(output[0] != 0); // Basic sanity check
    assert(output[0] == 0xa4386c6d); // Expected value from Set 6, vector# 3
}

void test_zero_key_iv() {
    const uint8_t key[10] = {0};
    const uint8_t iv[10] = {0};
    
    TRIVIUM_ctx ctx;
    TRIVIUM_init(&ctx, key, iv, 10, 10);
    
    uint32_t output[10];
    TRIVIUM_genkeystream32(&ctx, output, 10);
    
    // With zero key/iv, output should still be deterministic
    assert(output[0] != 0); // Should not be all zeros due to constant in LFSR C
}

void test_key_setup() {
    const uint8_t key[10] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a};
    const uint8_t iv[10] = {0};
    
    TRIVIUM_ctx ctx;
    TRIVIUM_keysetup(&ctx, key);
    ctx.keylen = 10;
    
    // Verify key is correctly stored
    for (int i = 0; i < 10; i++) {
        assert(ctx.key[i] == key[i]);
    }
}

void test_iv_setup() {
    const uint8_t key[10] = {0};
    const uint8_t iv[10] = {0x0a, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01};
    
    TRIVIUM_ctx ctx;
    TRIVIUM_ivsetup(&ctx, iv);
    ctx.ivlen = 10;
    
    // Verify IV is correctly stored
    for (int i = 0; i < 10; i++) {
        assert(ctx.iv[i] == iv[i]);
    }
}

void test_short_key_iv() {
    const uint8_t key[5] = {0xff, 0xee, 0xdd, 0xcc, 0xbb};
    const uint8_t iv[3] = {0x11, 0x22, 0x33};
    
    TRIVIUM_ctx ctx;
    TRIVIUM_init(&ctx, key, iv, 5, 3);
    
    uint32_t output[5];
    TRIVIUM_genkeystream32(&ctx, output, 5);
    
    // Should work with shorter keys/IVs
    assert(ctx.keylen == 5);
    assert(ctx.ivlen == 3);
}

void test_deterministic_output() {
    const uint8_t key[10] = {0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0, 0x11, 0x22};
    const uint8_t iv[10] = {0xab, 0xcd, 0xef, 0x01, 0x23, 0x45, 0x67, 0x89, 0xaa, 0xbb};
    
    TRIVIUM_ctx ctx1, ctx2;
    TRIVIUM_init(&ctx1, key, iv, 10, 10);
    TRIVIUM_init(&ctx2, key, iv, 10, 10);
    
    uint32_t output1[20], output2[20];
    TRIVIUM_genkeystream32(&ctx1, output1, 20);
    TRIVIUM_genkeystream32(&ctx2, output2, 20);
    
    // Same key/IV should produce same output
    for (int i = 0; i < 20; i++) {
        assert(output1[i] == output2[i]);
    }
}

void test_different_keys() {
    const uint8_t key1[10] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99};
    const uint8_t key2[10] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x98};
    const uint8_t iv[10] = {0};
    
    TRIVIUM_ctx ctx1, ctx2;
    TRIVIUM_init(&ctx1, key1, iv, 10, 10);
    TRIVIUM_init(&ctx2, key2, iv, 10, 10);
    
    uint32_t output1[10], output2[10];
    TRIVIUM_genkeystream32(&ctx1, output1, 10);
    TRIVIUM_genkeystream32(&ctx2, output2, 10);
    
    // Different keys should produce different output
    int different = 0;
    for (int i = 0; i < 10; i++) {
        if (output1[i] != output2[i]) {
            different = 1;
            break;
        }
    }
    assert(different);
}

int main() {
    test_set6_vector3();
    test_zero_key_iv();
    test_key_setup();
    test_iv_setup();
    test_short_key_iv();
    test_deterministic_output();
    test_different_keys();
    return 0;
}