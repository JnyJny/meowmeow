/* mmencode.c - MeowMeow, a stream encoder/decoder */

#include <errno.h>
#include <string.h>

#include "mmencode.h"

#define NL '\n'

int mm_encode(FILE *src, FILE *dst, int verbose)
{
  char buf[BUFSIZ];
  unsigned char hi;
  unsigned char lo;
  int           i;

  char * tbl[] = { "meow", "meoW", "meOw", "meOW",
		   "mEow", "mEoW", "mEOw", "mEOW",
		   "Meow", "MeoW", "MeOw", "MeOW",
		   "MEow", "MEoW", "MEOw", "MEOW" };
  
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
      lo = (buf[i] & 0x0f);
      hi = (buf[i] & 0xf0) >> 4;
      fputs(tbl[hi], dst);
      fputs(tbl[lo], dst);
    }
  }

  return 0;
}
