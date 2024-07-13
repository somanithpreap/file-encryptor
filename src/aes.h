#include <stdio.h>
#include <stdlib.h>

#define M 0x11B
#define AES128_NR 10 // Number of rounds for 128-bits key
#define AES192_NR 12 // Number of rounds for 192-bits key
#define ARS256_NR 14 // Number of rounds for 256-bits key

typedef unsigned char uint8;
typedef unsigned int uint32;

uint8 GF_add(uint8 a, uint8 b);
uint8 GF_mul(uint8 a, uint8 b);
uint8 __GF_mul__(uint8 a, uint8 b);

class State {
private:
  uint8 state[4][4];

public:
  State();
  State(uint8 msg[32]);
  void update(uint8 msg[32]);
  uint8 *word(uint8 index);
  uint8 get_byte(uint8 row_i, uint8 col_i);
  void set_byte(uint8 row_i, uint8 col_i, uint8 value);
  ~State();
};

class AES {
private:
  State state;
  uint8 *key;

public:
  AES(uint8 *msg, uint8 *key);
  ~AES();
};
