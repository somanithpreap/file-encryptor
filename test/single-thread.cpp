#include "../src/aes.h"

int main() {
  uint8 key[16] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
  uint8 input[16] = {0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a, 0x00, 0x00, 0x00, 0x0d, 0x49, 0x48, 0x44, 0x52};

  // Initialize AES instances and create round keys
  AES instance(16);
  uint8 round_keys[44][4];
  key_expansion(16, key, round_keys);

  printf("Original Buffer: ");
  display_buffer(input);

  uint8 holder_e[16];
  printf("After Encryption:\n");
  instance.encrypt(input, holder_e, round_keys);
  display_buffer(holder_e);

  uint8 holder_d[16];
  printf("After Decryption:\n");
  instance.decrypt(holder_e, holder_d, round_keys);
  display_buffer(holder_d);
}
