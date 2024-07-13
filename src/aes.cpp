#include "aes.h"
/*
 * procedure CIPHER(msg, Nr, w)
 *   state <- msg
 *   state <- AddRoundKey(state,w[0...3])
 *   for round from 1 to Nr-1 do
 *     state <- SubBytes(state)
 *     state <- ShiftRows(state)
 *     state <- MixColumns(state)
 *     state <- AddRoundKey(state,w[4*round...4*round+3])
 *   end for
 *   state <- SubBytes(state)
 *   state <- ShiftRows(state)
 *   state <- AddRoundKey(state,w[4*Nr...4*Nr+3])
 *   return state
 * end procedure
 */

// Addition in GF(2^8)
uint8 GF_add(uint8 a, uint8 b) { return a ^ b; }

// Multiplication in GF(2^8)
uint8 GF_mul(uint8 a, uint8 b) {
  uint8 p = 0; // Multiplication product accumulator

  for (uint8 i = 0; i <= 8; i++) {
    p ^= -(b & 1) & a;
    b >>= 1;
    auto carry = -(a >> 7);
    a = (uint8)((a << 1) ^ (M & carry));
  }

  return p;
}

uint8 __GF_mul__(uint8 a, uint8 b) {
  uint8 p = 0; // Multiplication product accumulator
  while (a != 0 && b != 0) {
    if (b & 1)
      p ^= a;
    b >>= 1;
    a = (uint8)((a << 1) ^ (M & -(a >> 7)));
  }
  return p;
}

State::State() {
  for (uint8 c = 0; c < 4; c++) {
    for (uint8 r = 0; r < 4; r++)
      state[r][c] = 0x00;
  }
}

State::State(uint8 msg[16]) {
  if (msg[0] == 0) {
    printf(
        "[ERR] State::State(): Message must be a non-zero buffer sequence\n");
    exit(-1);
  }

  for (uint8 c = 0; c < 4; c++) {
    for (uint8 r = 0; r < 4; r++)
      state[r][c] = msg[r + 4 * c];
  }
}

State::~State() { free(this); }

AES::AES(uint8 *msg, uint8 *key) {
  // if (strlen(msg) > 16) {
  //   printf("Invalid message size!\n");
  //   abort();
  // }
  // if (strlen(key) == 0 || strlen(key) > 32) {
  //   printf("Invaild key size!\n");
  //   abort();
  // }
}

AES::~AES() { free(this); }
