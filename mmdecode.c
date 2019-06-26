/* mmdecode.c - MeowMeow, a stream decoder/decoder */

#include <errno.h>
#include <string.h>

#include "mmdecode.h"


int mm_decode(FILE *src, FILE *dst, int verbose)
{
  char buf[BUFSIZ];
  unsigned char c;
  unsigned int  w;
  int           b;
  int           i;
  int           j;
  
  if (!src || !dst) {
    errno = EINVAL;
    return -1;
  }

  while (!feof(src)) {
    if (!fgets(buf, sizeof(buf), src)) {
      ferror(src);
      return -1;
    }
    
    for (i=0;i<strlen(buf);i++) {
      c = buf[i];
      for (j=7;j>0;j--){
	

      }
    }

  }

  return 0;
}

int decoder(char c)
{
  switch(c) {
    case 'M':
    case 'E':
    case 'O':
    case 'W':
      return 1;
    case 'm':
    case 'e':
    case 'o':
    case 'w':
      return 0;
    default:
      fprintf(stderr, "mmdecode:decoder encountered error %c", c);
  }

  return -1;
}
