/* mmdecode.c - MeowMeow, a stream encoder/decoder */

#include <errno.h>
#include <string.h>
#include <ctype.h>

#include "mmdecode.h"

typedef struct {
  unsigned char f7:1;    
  unsigned char f6:1;
  unsigned char f5:1;
  unsigned char f4:1;    
  unsigned char f3:1;
  unsigned char f2:1;
  unsigned char f1:1;    
  unsigned char f0:1;
} fields_t;

typedef union {
  fields_t      field;
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
    
    if (!fgets(buf, sizeof(buf), src))
      break;

    byte.field.f0 = isupper(buf[0])==0?0:1;
    byte.field.f1 = isupper(buf[1])==0?0:1;
    byte.field.f2 = isupper(buf[2])==0?0:1;
    byte.field.f3 = isupper(buf[3])==0?0:1;
    byte.field.f4 = isupper(buf[4])==0?0:1;
    byte.field.f5 = isupper(buf[5])==0?0:1;
    byte.field.f6 = isupper(buf[6])==0?0:1;
    byte.field.f7 = isupper(buf[7])==0?0:1;
    
    fputc(byte.value, dst);
  }

  return 0;
}

