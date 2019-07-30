/* mmencode.c - MeowMeow, a stream encoder/decoder */

#include <errno.h>
#include <string.h>

#include "mmencode.h"
#include "table.h"

#define LO_MASK 0x000f
#define HI_MASK 0x00f0
#define BITS_IN_NIBBLE 4

int mm_encode(FILE *src, FILE *dst)
{
  char buf[BUFSIZ];
  unsigned char hi;
  unsigned char lo;
  int           i;

  char * tbl[] = ENCODER_INIT;
  
  if (!src || !dst) {
    errno = EINVAL;
    return -1;
  }

  while (!feof(src)) {
    
    if (!fgets(buf, sizeof(buf), src))
      break;

    for(i=0; i<strlen(buf); i++) {
      lo = (buf[i] & LO_MASK);
      hi = (buf[i] & HI_MASK) >> BITS_IN_NIBBLE;
      fputs(tbl[hi], dst);
      fputs(tbl[lo], dst);
    }
  }

  return 0;
}
