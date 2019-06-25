/* mmdecode.c - MeowMeow, a stream decoder/decoder */

#include <errno.h>
#include "mmdecode.h"


int mm_decode(FILE *src, FILE *dst, int verbose)
{
  if (!src || !dst) {
    errno = EINVAL;
    return -1;
  }

  return 0;
}
