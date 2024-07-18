#include "aes.h"
#include <cstdio>

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

void sub_byte(uint8 *byte) {
  CHECK_NULL_PTR(byte);

  uint8 s_row = *byte >> 4;
  uint8 s_col = (uint8)(*byte << 4) >> 4;
  *byte = SBOX[s_row][s_col];
}

void sub_word(uint8 word[4]) {
  for (uint8 i = 0; i < 4; i++)
    sub_byte(&word[i]);
}

State::State() {
  // Populate the bytes of State with 0
  for (uint8 r = 0; r < 4; r++) {
    for (uint8 c = 0; c < 4; c++)
      this->state[r][c] = 0;
  }
}

State::State(uint8 in[16]) { this->update(in); }

// Populate the bytes of State with bytes of input data
void State::update(uint8 in[16]) {
  CHECK_NON_ZERO_BUFFER(16, in);
  for (uint8 r = 0; r < 4; r++) {
    for (uint8 c = 0; c < 4; c++)
      this->state[r][c] = in[r + 4 * c];
  }
}

uint8 State::get_byte(uint8 row_i, uint8 col_i) {
  if (row_i > 3 || col_i > 3)
    ERROR("State::get_byte(): Invalid row or column index [%hhu,%hhu].", row_i,
          col_i);
  return this->state[row_i][col_i];
}

void State::get_word(uint8 index, uint8 holder[4]) {
  if (index > 3)
    ERROR("State::get_word(): Invalid word index [%hhu].", index);
  CHECK_NULL_PTR(holder);
  for (uint8 i = 0; i < 4; i++)
    holder[i] = this->state[i][index];
}

void State::get_state(uint8 holder[4][4]) {
  CHECK_NULL_PTR(holder);
  for (uint8 r = 0; r < 4; r++) {
    for (uint8 c = 0; c < 4; c++)
      holder[r][c] = this->state[r][c];
  }
}

void State::set_byte(uint8 row_i, uint8 col_i, uint8 value) {
  if (row_i > 3 || col_i > 3)
    ERROR("State::set_byte(): Invalid row or column index [%hhu,%hhu].", row_i,
          col_i);
  this->state[row_i][col_i] = value;
}

void State::serialize(uint8 holder[16]) {
  CHECK_NULL_PTR(holder);
  for (uint8 r = 0; r < 4; r++) {
    for (uint8 c = 0; c < 4; c++)
      holder[r + 4 * c] = this->state[r][c];
  }
}

void State::print_state() {
  printf("+-------------+\n");
  for (uint8 i = 0; i < 4; i++) {
    printf("| ");
    for (uint8 j = 0; j < 4; j++)
      printf("%.2X ", this->state[i][j]);
    printf("|\n");
  }
  printf("+-------------+\n");
}

void State::add_round_key(uint8 r_key[16]) {
  CHECK_NON_ZERO_BUFFER(16, r_key);
  for (uint8 r = 0; r < 4; r++) {
    for (uint8 c = 0; c < 4; c++)
      this->state[r][c] ^= r_key[r + 4 * c];
  }
}

void State::sub_bytes() {
  for (uint8 r = 0; r < 4; r++) {
    for (uint8 c = 0; c < 4; c++)
      sub_byte(&(this->state[r][c]));
  }
}

void State::shift_rows() {
  l_rotate_word(this->state[1], 1);

  uint8 tmp = this->state[2][0];
  this->state[2][0] = this->state[2][2];
  this->state[2][2] = tmp;
  tmp = this->state[2][1];
  this->state[2][1] = this->state[2][3];
  this->state[2][3] = tmp;

  r_rotate_word(this->state[3], 1);
}

void State::mix_columns() {
  for (uint8 i = 0; i < 4; i++) {
    uint8 word[4];
    this->get_word(i, word);
    for (uint8 r = 0; r < 4; r++) {
      uint8 tmp = 0;
      for (uint8 c = 0; c < 4; c++)
        tmp ^= GF_mul(MIXCOL_MATRIX[r][c], word[c]);
      this->state[i][r] = tmp;
    }
  }
}

State::~State() {}

AES::AES(uint8 in[16], uint8 key[32]) {}

AES::~AES() {}
