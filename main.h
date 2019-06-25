/* main.h - MeowMeow, a stream encoder/decoder */

#ifndef _MAIN_H
#define _MAIN_H

#define MEOWMEOW_VERSION "0.1.0"

#include <stdio.h>

enum {
      QUIET=0,
      TERSE=1,
      CHATTY=2,
};

typedef struct options
{
  FILE *in_stream;
  FILE *out_stream;
  int   verbose;
} options_t;

#define OPTIONS_INIT { stdin, stdout, QUIET }

#endif	/* _MAIN_H */
