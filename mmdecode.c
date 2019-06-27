/* mmdecode.c - MeowMeow, a stream decoder/decoder */

#include <errno.h>
#include <string.h>
#include <ctype.h>

#include "mmdecode.h"

typedef union {
  struct {
#if __BYTE_ORDER != __BIG_ENDIAN
    unsigned int f0:1;    
    unsigned int f1:1;
    unsigned int f2:1;
    unsigned int f3:1;    
    unsigned int f4:1;
    unsigned int f5:1;
    unsigned int f6:1;    
    unsigned int f7:1;
#else
    unsigned int f7:1;    
    unsigned int f6:1;
    unsigned int f5:1;
    unsigned int f4:1;    
    unsigned int f3:1;
    unsigned int f2:1;
    unsigned int f1:1;    
    unsigned int f0:1;
#endif
  } field;
  unsigned char value;
} decoded_byte_t;

int stupid_decode(FILE *src, FILE *dst);

int mm_decode(FILE *src, FILE *dst)
{
  if (!src || !dst) {
    errno = EINVAL;
    return -1;
  }

  return stupid_decode(src, dst);
}

int stupid_decode(FILE *src, FILE *dst)
{
  char           buf[9];
  decoded_byte_t byte;
  int            i;
  
  while (!feof(src)) {
    
    if (!fgets(buf, sizeof(buf), src)) {
      ferror(src);
      return -1;
    }

    byte.field.f0 = isupper(buf[0]);
    byte.field.f1 = isupper(buf[1]);
    byte.field.f2 = isupper(buf[2]);
    byte.field.f3 = isupper(buf[3]);
    byte.field.f4 = isupper(buf[4]);
    byte.field.f5 = isupper(buf[5]);
    byte.field.f6 = isupper(buf[6]);
    byte.field.f7 = isupper(buf[7]);    
    
    fputc(byte.value, dst);
  }

  return 0;
}

