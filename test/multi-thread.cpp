#include "../src/aes.h"
#include <thread>
using namespace std;

#define NUM_THREADS 2

int main() {
  // A single threaded test of encrypting 64 bytes of data using one AES
  // instance
  uint8 key[16] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
                   0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};

  uint8 inputs[4][16] = {{0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30, 0x8d, 0x31,
                          0x31, 0x98, 0xa2, 0xe0, 0x37, 0x07, 0x34},
                         {0x4e, 0x34, 0x50, 0xe3, 0x11, 0x86, 0x2b, 0xef, 0x74,
                          0x46, 0xb0, 0xb4, 0xe1, 0x6c, 0x59, 0x16},
                         {0x4b, 0xa8, 0xa7, 0x21, 0x01, 0x46, 0x8e, 0x09, 0x9e,
                          0x98, 0x79, 0x18, 0x3f, 0x8a, 0x66, 0xdd},
                         {0x41, 0x7d, 0x77, 0xf6, 0x09, 0xa4, 0x16, 0x8d, 0x1a,
                          0x28, 0x1e, 0xb8, 0x8e, 0xcd, 0xfb, 0x13}};

  // Initialize AES instances and create round keys
  AES<16> instances[NUM_THREADS];
  uint8 round_keys[AES128_NR + 1][4][4];
  key_expansion(16, key, round_keys);

  printf("Original Buffer:  ");
  for (uint8 i = 0; i < 4; i++)
    display_buffer(inputs[i]);

  // Thread handling
  thread *threads[NUM_THREADS];
  uint8 holders[4][16];

  printf("After Encryption:\n");
  for (uint8 i = 0; i < 2; i++) {
    for (uint8 j = 0; j < NUM_THREADS; j++) {
      instances[2 * i + j].update(inputs[2 * i + j]);
      threads[j] = new thread(&AES<16>::encrypt, instances[2 * i + j].state,
                              holders[2 * i + j], round_keys);
    }
    for (uint8 j = 0; j < NUM_THREADS; j++) {
      threads[j]->join();
      display_buffer(holders[2 * i + j]);
    }
  }
}
