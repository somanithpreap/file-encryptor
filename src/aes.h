#include "utils.h"

uint8 GF_add(uint8 a, uint8 b);
uint8 GF_mul(uint8 a, uint8 b); // Recommended to use this instead of __GF_mul__
uint8 __GF_mul__(uint8 a, uint8 b);

class State {
private:
  uint8 state[16];

public:
  State();
  State(uint8 in[16]);
  void update(uint8 in[16]);
  uint8 get_byte(uint8 row_i, uint8 col_i);
  uint32 get_word(uint8 index);
  uint8 *get_state();
  void set_byte(uint8 row_i, uint8 col_i, uint8 value);
  ~State();
};

class AES {
private:
  State state;
  uint8 key[32];

public:
  AES(uint8 in[16], uint8 key[32]);
  void update(uint8 in[16]);
  uint8 *encrypt();
  uint8 *decrypt();
  ~AES();
};
