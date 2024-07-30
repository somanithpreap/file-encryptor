#include "aes.h"

int main() {
  // Before testing for the output of key_expansion, you should undo all the
  // comment lines that has working code first
  uint8 input[16] = {45, 137, 223, 14, 56, 78,  190, 123,
                     9,  200, 176, 89, 34, 150, 2,   73};
  uint8 key[32] = {0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe,
                   0x2b, 0x73, 0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81,
                   0x1f, 0x35, 0x2c, 0x07, 0x3b, 0x61, 0x08, 0xd7,
                   0x2d, 0x98, 0x10, 0xa3, 0x09, 0x14, 0xdf, 0xf4};
  AES<32> instance0(input, key);
<<<<<<< HEAD
  instance0.key_expansion(key);
=======
  instance0.key_expansion();
  
>>>>>>> fd88c36 (Changing round_key from 2D to 3D array and some modifications within key_expansion())
}
