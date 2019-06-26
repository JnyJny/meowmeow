/* mmencode.c - MeowMeow, a stream encoder/decoder */

#include <errno.h>
#include <string.h>

#include "mmencode.h"
#include "table.h"

int mm_encode(FILE *src, FILE *dst)
{
  char buf[BUFSIZ];
  unsigned char *byte;
  unsigned char hi;
  unsigned char lo;
  int           i;

  char * tbl[] = ENCODER_INIT;
  
  if (!src || !dst) {
    errno = EINVAL;
    return -1;
  }

  while (!feof(src)) {
    
    if (!fgets(buf, sizeof(buf), src)) {
      ferror(src);
      return -1;
    }

    for(i=0; i<strlen(buf); i++) {
      lo = (buf[i] & 0x000f);
      hi = (buf[i] & 0x00f0) >> 4;
      fputs(tbl[hi], dst);
      fputs(tbl[lo], dst);
    }
  }

  return 0;
}
