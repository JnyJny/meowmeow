/* mmencode.c - MeowMeow, a stream encoder/decoder */

#include <errno.h>
#include "mmencode.h"


int mm_encode(FILE *src, FILE *dst, int verbose)
{
  if (!src || !dst) {
    errno = EINVAL;
    return -1;
  }

  return 0;
}
