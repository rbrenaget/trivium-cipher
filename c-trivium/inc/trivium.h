/**
 * @file trivium.h
 * @author Romain Brenaget
 * @brief 
 * @version 0.1
 * @date 2020-03-03
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef TRIVIUM
#define TRIVIUM

//#include <stdlib.h>
//#include <limits.h>
#include <stdint.h>

typedef struct {
  uint8_t keylen;
  uint8_t ivlen;

  uint8_t states[36];
  uint8_t key[10];
  uint8_t iv[10];

} TRIVIUM_ctx;


int TRIVIUM_init(TRIVIUM_ctx* ctx, const uint8_t* key, const uint8_t* iv, uint8_t keylen, uint8_t ivlen);
void TRIVIUM_keysetup(TRIVIUM_ctx* ctx, const uint8_t* key);
void TRIVIUM_ivsetup(TRIVIUM_ctx* ctx, const uint8_t* iv);

#endif