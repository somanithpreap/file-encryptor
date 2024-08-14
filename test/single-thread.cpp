#include "../src/aes.h"

int main() {
  // A single threaded test of encrypting 64 bytes of data using one AES
  // instance
  uint8 key[16] = {0x2b, 0x7e, 0x15, 0x16,
                   0x28, 0xae, 0xd2, 0xa6,
                   0xab, 0xf7, 0x15, 0x88,
                   0x09, 0xcf, 0x4f, 0x3c};

  uint8 inputs[4][4] = {{0x32, 0x43, 0xf6, 0xa8},
                        {0x88, 0x5a, 0x30, 0x8d},
                        {0x31, 0x31, 0x98, 0xa2},
                        {0xe0, 0x37, 0x07, 0x34}};

  // Initialize AES instances and create round keys
  AES instance(16);
  uint8 round_keys[(AES128_NR + 1) * 4][4];
  key_expansion(16, key, round_keys);

  // printf("Original Buffer:\n");
  // for (uint8 i = 0; i < 16; i++)
  //   display_buffer(inputs[i]);

  // uint8 holder[16];
  // printf("After Decryption:\n");
  // for (uint8 i = 0; i < 16; i++) {
  //   instance.decrypt(inputs[i], holder, round_keys);
  //   display_buffer(holder);
  // }
}
