#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#define M 0x11B
#define AES128_NR 10 // Number of rounds for 128-bits key
#define AES192_NR 12 // Number of rounds for 192-bits key
#define ARS256_NR 14 // Number of rounds for 256-bits key

typedef unsigned char uint8;
typedef unsigned int uint32;

void ERROR(const char *message, ...);
void CHECK_NULL_PTR(void *ptr);
bool CHECK_NON_ZERO_BUFFER(size_t length, uint8 *buf);
