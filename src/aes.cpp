#include "aes.h"

/* Multiplication in GF(2^8)
Parameters: uint8 a: left operand
            uint8 b: right operand
Returns: the result of the Finite Field multiplication
*/
uint8 GF_mul(uint8 a, uint8 b) {
  uint8 p = 0; // Multiplication product accumulator
  for (uint8 i = 0; i <= 8; i++) {
    p ^= -(b & 1) & a;
    b >>= 1;
    uint8 carry = -(a >> 7);
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

/* Substitute a byte with another byte using the SBOX and Inverse SBOX
Parameters: bool inverse: true for inverse substitution; false for normal
substitution uint8 *byte: reference to the byte to be substituted
*/
void sub_byte(bool inverse, uint8 *byte) {
  CHECK_NULL_PTR(byte);
  uint8 s_row = *byte >> 4;
  uint8 s_col = (uint8)(*byte << 4) >> 4;
  *byte = ((inverse) ? InvSBOX : SBOX)[s_row][s_col];
}

/* Substitute each byte in a given word using the sub_byte()
Parameters: bool inverse: true for inverse substitution; false for normal
substitution uint8 word[4]: reference to the word to be substituted
*/
void sub_word(bool inverse, uint8 word[4]) {
  for (uint8 i = 0; i < 4; i++)
    sub_byte(inverse, &word[i]);
}

// State class constructor to initialize a State with all zero-value bytes
State::State() {
  // Populate the bytes of State with 0
  for (uint8 r = 0; r < 4; r++) {
    for (uint8 c = 0; c < 4; c++)
      this->state[r][c] = 0;
  }
}

/* State class constructor to initialize a State with given input byte sequence
Parameter: uint8 in[16]: 128-bit byte sequence to be unserialized
*/
State::State(uint8 in[16]) { this->update(in); }

// Populate the bytes of State with bytes of input data
void State::update(uint8 in[16]) {
  CHECK_NULL_PTR(in);
  for (uint8 c = 0; c < 4; c++) {
    for (uint8 r = 0; r < 4; r++)
      this->state[r][c] = in[r + 4 * c];
  }
}

/* Retrieve a byte from the State
Parameters: uint8 row_i: row index of the byte
            uint8 col_i: column index of the byte
Returns: a byte from the State specified by the row and column index
*/
uint8 State::get_byte(uint8 row_i, uint8 col_i) {
  if (row_i > 3 || col_i > 3)
    ERROR("State::get_byte(): Invalid row or column index [%hhu,%hhu].", row_i,
          col_i);
  return this->state[row_i][col_i];
}

/* Retrieve a word from the State
Parameters: uint8 index: index of the word
            uint8 holder[4]: a holder variable for the word
*/
void State::get_word(uint8 index, uint8 holder[4]) {
  if (index > 3)
    ERROR("State::get_word(): Invalid word index [%hhu].", index);
  CHECK_NULL_PTR(holder);
  for (uint8 i = 0; i < 4; i++)
    holder[i] = this->state[i][index];
}

/* Retrieve the whole data of the state
Parameter: uint8 holder[4][4]: a holder variable for the State
*/
void State::get_state(uint8 holder[4][4]) {
  CHECK_NULL_PTR(holder);
  for (uint8 r = 0; r < 4; r++) {
    for (uint8 c = 0; c < 4; c++)
      holder[r][c] = this->state[r][c];
  }
}

/* Set a byte in the state to a specified value
Parameters: uint8 row_i: row index of the byte
            uint8 col_i: column index of the byte
            uint8 value: value to set the specified byte to
*/
void State::set_byte(uint8 row_i, uint8 col_i, uint8 value) {
  if (row_i > 3 || col_i > 3)
    ERROR("State::set_byte(): Invalid row or column index [%hhu,%hhu].", row_i,
          col_i);
  this->state[row_i][col_i] = value;
}

/* Transform the state into a buffer sequence
Parameter: uint8 holder[16]: a holder variable for the buffer sequence
*/
void State::serialize(uint8 holder[16]) {
  CHECK_NULL_PTR(holder);
  for (uint8 r = 0; r < 4; r++) {
    for (uint8 c = 0; c < 4; c++)
      holder[r + 4 * c] = this->state[r][c];
  }
}

void State::print() {
  printf("+-------------+\n");
  for (uint8 i = 0; i < 4; i++) {
    printf("| ");
    for (uint8 j = 0; j < 4; j++)
      printf("%.2X ", this->state[i][j]);
    printf("|\n");
  }
  printf("+-------------+\n");
}

/* XOR each byte in the State by each respective byte in the given round key
Parameter: uint8 r_key[4][4]: reference to the round key in a 2D array format
*/
void State::add_round_key(uint8 (*round_keys)[4], uint8 round) {
  for (uint8 c = 0; c < 4; c++) {
    for (uint8 r = 0; r < 4; r++) this->state[c][r] ^= round_keys[4*round+c][r];
  }
}

/* Substitute each byte in the State using the sub_byte()
Parameter: bool inverse: true for inverse substitution; false for normal
substitution
*/
void State::sub_bytes(bool inverse) {
  for (uint8 r = 0; r < 4; r++) {
    for (uint8 c = 0; c < 4; c++)
      sub_byte(inverse, &(this->state[r][c]));
  }
}

/* Rotate shift each row in the State by its row index
Parameter: bool inverse: true for rotating to the right; false for rotatign to
the left
*/
void State::shift_rows(bool inverse) {
  void (*rot_word[2])(uint8 *, uint8) = {l_rotate_word, r_rotate_word};

  rot_word[(inverse) ? 1 : 0](this->state[1], 1);

  uint8 tmp = this->state[2][0];
  this->state[2][0] = this->state[2][2];
  this->state[2][2] = tmp;
  tmp = this->state[2][1];
  this->state[2][1] = this->state[2][3];
  this->state[2][3] = tmp;

  rot_word[(inverse) ? 0 : 1](this->state[3], 1);
}

/* Matrix multiplication of the State with a fixed matrix
Parameter: bool inverse: true for multiplication with InvMIXCOL_MATRIX; false
for multiplication with MIXCOL_MATRIX
*/
void State::mix_columns(bool inverse) {
  for (uint8 w = 0; w < 4; w++) {
    uint8 word[4];
    this->get_word(w, word);
    for (uint8 r = 0; r < 4; r++) {
      uint8 tmp = 0;
      for (uint8 c = 0; c < 4; c++)
        tmp ^= GF_mul(((inverse) ? InvMIXCOL_MATRIX : MIXCOL_MATRIX)[r][c],
                      word[c]);
      this->state[r][w] = tmp;
    }
  }
}

void key_expansion(uint8 k_len, uint8 *key, uint8 (*holder)[4]) {
  CHECK_NULL_PTR(key);
  CHECK_NULL_PTR(holder);

  uint8 Nk = k_len / 4;
  uint8 Nr = 6 + Nk;

  uint8 i = 0;
  // printf("Output of the first 8 round keys\n");
  while (i < Nk) {
    printf("round_key %i: ", i);
    for (uint8 j = 0; j < 4; j++) {
      holder[i][j] = key[4 * i + j];
      // printf("%.2x", holder[i][j]);
    }
    printf("\n");
    i++;
  }
  //i = 8 at the end of 1st while loop

  //This loops is for 1D array 'temp' taking the value of 2D array 'round_key' at (i - 1) row which is simply word.

  printf("Output of the next 52 round keys\n");
  uint8 temp[4];

  while (i <= 4 * Nr + 3) {
    //printf("\n");
    //printf("temp %d : ", i - 1);
    for (uint8 j = 0; j < 4; j++) {
      temp[j] = holder[i - 1][j];
      //printf("%.2x", temp[j]);
    }
    //printf("\n");


    if (i % Nk == 0) {
      /*This condition left rotates the byte of word, substitute it with SBOX
      and do XOR operation on word located at (i % Nk == 0) with Rcon.*/
      l_rotate_word(temp, 1);
      //printf("after l_rotate temp %d : ", i - 1);
      //for (uint8 j = 0; j < 4; j++) {
      //  printf("%.2x", temp[j]);
      //}
      //printf("\n");

      sub_word(false, temp);
      //printf("after substitute temp %d : ", i - 1);
      //for (uint8 j = 0; j < 4; j++) {
      //  printf("%.2x", temp[j]);
      //}
      //printf("\n");

      for (uint8 j = 0; j < 4; j++) {
        temp[j] ^= Rcon[(i - 1) / Nk][j];
      }

      //printf("after Rcon temp %d : ", i - 1);
      //for (uint8 j = 0; j < 4; j++) {
      //  printf("%.2x", temp[j]);
      //}
      //printf("\n");

    } else if (Nk > 6 && i % Nk == 4) sub_word(false, temp);
      /*This condition is only for AES-256.
      At (i % Nk == 4), 'temp' will be substituted by Sbox*/

      //printf("l_rotated temp %d : ", i - 1);
      //for (uint8 j = 0; j < 4; j++) {
      //  printf("%.2x", temp[j]);
      //}
      //printf("\n");

    /*This loop does XOR operation between each elements of round_key at (i - Nk) and temp.
    It then takes append 'temp' into 'round_key' at (i)th row.*/
    printf("round_key %d : ", i);
    for (uint8 j = 0; j < 4; j++) {
      holder[i][j] = holder[i - Nk][j] ^ temp[j];
      printf("%.2x", holder[i][j]);
    }
    printf("\n");

    i++;
  }
}

AES::AES(uint8 k_len) {
  if (k_len == 16 || k_len == 24 || k_len == 32) this->rounds = 6 + k_len / 4;
  else ERROR("AES::AES(): Invalid key length.");
}

/* Encrypt the current data in the State and outputs data to a holder
Parameters: uint8 data[16]: data to be encrypted
            uint8 holder[16]: holder variable for the encrypted data
            uint8 round_key[7 + k_len / 4][4][4]: array of round keys */
void AES::encrypt(uint8 data[16], uint8 holder[16],
                         uint8 (*round_key)[4]) {
  CHECK_NULL_PTR(data);
  CHECK_NULL_PTR(holder);
  CHECK_NULL_PTR(round_key);

  this->state.update(data);
  this->state.add_round_key(round_key, rounds);
  for (uint8 i = 1; i < this->rounds; i++) {
    this->state.sub_bytes(false);
    this->state.shift_rows(false);
    this->state.mix_columns(false);
    this->state.add_round_key(round_key, rounds);
  }
  this->state.sub_bytes(false);
  this->state.shift_rows(false);
  this->state.add_round_key(round_key, rounds);
  this->state.serialize(holder);
}

/* Decrypt the current data in the this->state and outputs data to a holder
Parameters: uint8 data[16]: data to be decrypted
            uint8 holder[16]: holder variable for the decrypted data
            uint8 round_key[7 + k_len / 4][4][4]: array of round keys */
void AES::decrypt(uint8 data[16], uint8 holder[16],
                         uint8 (*round_key)[4]) {
  CHECK_NULL_PTR(data);
  CHECK_NULL_PTR(holder);
  CHECK_NULL_PTR(round_key);

  this->state.update(data);
  this->state.add_round_key(round_key, rounds);
  this->state.shift_rows(true);
  this->state.sub_bytes(true);
  for (uint8 i = this->rounds - 1; i > 0; i--) {
    this->state.add_round_key(round_key, rounds);
    this->state.mix_columns(true);
    this->state.shift_rows(true);
    this->state.sub_bytes(true);
  }
  this->state.add_round_key(round_key, rounds);
  this->state.serialize(holder);
}
