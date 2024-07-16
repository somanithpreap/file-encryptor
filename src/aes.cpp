#include "aes.h"
/*
 * procedure CIPHER(in, Nr, w)
 *   state <- in
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
  // Populate the bytes of State with 0
  for (uint8 i = 0; i < 16; i++)
    state[i] = 0;
}

State::State(uint8 in[16]) {
  // Check if the input data is a non-zero buffer sequence
  if (CHECK_NON_ZERO_BUFFER(16, in)) {
    ERROR("State::State(): Message must be a non-zero buffer sequence.");
  }

  // Populate the bytes of State with bytes of input data
  for (uint8 i = 0; i < 16; i++)
    state[i] = in[i];
}

State::~State() {}

AES::AES(uint8 in[16], uint8 key[32]) {}

AES::~AES() {}
