/* main.c - MeowMeow, a stream encoder/decoder */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <libgen.h>
#include <string.h>
#include <errno.h>
#include <getopt.h>

#include "main.h"
#include "mmencode.h"
#include "mmdecode.h"

extern char *optarg;
extern int   opterr;
extern int   opterr;

#define OPTARG "i:o:hvV"

#define MM_OP_INVALID 0
#define MM_OP_ENCODE  1
#define MM_OP_DECODE  2

int usage(char *, int);
int get_operation(char *);


int main(int argc, char *argv[])
{
  options_t options = OPTIONS_INIT;
  int opt;
  int retval;

  opterr = 0;
  
  while( (opt=getopt(argc, argv, OPTARG)) != EOF)
    switch(opt) {
      case 'i':
	/* XXX handle "-" explicitly as stdin */
	if (!(options.in_stream = fopen(optarg, "r"))) {
	  perror("Error opening input stream");
	  exit(EXIT_FAILURE);
	  /* NOTREACHED */
	}
	break;
	
      case 'o':
	/* XXX handle "-" explicitly as stdout */
	if (!(options.out_stream = fopen(optarg, "w"))) {
	  perror("Error opening output stream");
	  exit(EXIT_FAILURE);
	  /* NOTREACHED */
	}
	break;
	
      case 'v':
	options.verbose ++;
	break;

      case 'V':
	printf("%s version %s\n",
	       basename(argv[0]),
	       MEOWMEOW_VERSION);
	exit(EXIT_SUCCESS);
	/* NOTREACHED */
	break;
	
      case '?':
      case 'h':
      default:
	usage(basename(argv[0]), opt);
	break;
    }

  switch(get_operation(argv[0])) {
    case MM_OP_ENCODE:
      retval = mm_encode(options.in_stream, options.out_stream, options.verbose);
      break;
      
    case MM_OP_DECODE:
      retval = mm_decode(options.in_stream, options.out_stream, options.verbose);      
      break;
      
    default:
      retval = -1;
      errno = EINVAL;
      /* XXX write something snarky here and abend */
      break;
  }

  return retval==0?EXIT_SUCCESS:EXIT_FAILURE;
}


int usage(char *argv0, int opt)
{
  if (!argv0) {
    errno = EINVAL;
    perror("main:usage called with NULL argv[0]");
    exit(EXIT_FAILURE);
  }
  
  fprintf(stderr, "usage: %s [-i input] [-o output] [-v]\n",
	  basename(argv0));
  fprintf(stderr, "\n\tunknown option %c\n", opt);
  exit(EXIT_FAILURE);
  /* NOTREACHED */
}


int get_operation(char *argv0)
{
  if (!argv0) {
    errno = EINVAL;
    return MM_OP_INVALID;
  }
  
  switch(basename(argv0)[0]) {
    case 'm':
      return MM_OP_ENCODE;
      /* NOTREACHED */
    case 'u':
      return MM_OP_DECODE;
      /* NOTREACHED */
    default:
      break;
  }
  
  return MM_OP_INVALID;
}
