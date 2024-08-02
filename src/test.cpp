#include "aes.h"

void display_buffer(uint8 buf[16]) {
  for (uint8 i = 0; i < 16; i++)
    printf("%.2X ", buf[i]);
  printf("\n");
}

int main() {
  // Before testing for the output of key_expansion, you should undo all the
  // comment lines that has working code first
  uint8 input[16] = {0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30, 0x8d,
                     0x31, 0x31, 0x98, 0xa2, 0xe0, 0x37, 0x07, 0x34};
  uint8 key[16] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
                   0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
  AES<16> instance0(input, key);
  instance0.key_expansion();

  printf("Original Buffer:  ");
  display_buffer(input);

  uint8 holder[16];
  printf("After Encryption: ");
  instance0.encrypt(holder);
  display_buffer(holder);

  printf("After Decryption: ");
  instance0.decrypt(holder);
  display_buffer(holder);
}
