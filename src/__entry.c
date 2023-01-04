#include "h2o.h"
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char ** argv)
{
  if (argc < 2) {
    fprintf(stderr, "fatal: no input\n") ;
    fprintf(stderr, "usage: %s INPUT -o OUTPUT\n", argv[0]) ;
    exit(EXIT_FAILURE) ;
  }

  struct {
    int    type   ;
    char * input  ;
    char * output ;
  } opts ;

  opts.type   = 0    ;
  opts.input  = NULL ;
  opts.output = NULL ;

  for (int i = 1 ; i < argc ; ++i) {
    if (0 == str_comp(argv[i], "-o")) {
      if (i + 1 == argc) {
        fprintf(stderr, "fatal: missing argument for option `%s`\n", argv[i]) ;
        exit(EXIT_FAILURE) ;
      }

      ++i ;

      opts.output = argv[i] ;
    } else {
#ifdef _EMIT_WARNINGS
      if (NULL != opts.input)
        fprintf(stderr, "warning: ignored input `%s`\n", argv[i]) ;
      else
#endif
        opts.input = argv[i] ;
    }
  }

  if (NULL == opts.input) {
    fprintf(stderr, "fatal: no input\n") ;
    fprintf(stderr, "usage: %s INPUT -o OUTPUT\n", argv[0]) ;
    exit(EXIT_FAILURE) ;
  }

  char * default_output = "a.o" ;

  if (NULL == opts.output)
    opts.output = default_output ;

  if (H2O_SUCCESS != h2o(opts.input, opts.output)) {
    fprintf(stderr, "Something has gone wrong...\n") ;
    exit(EXIT_FAILURE) ;
  }
  
  exit(EXIT_SUCCESS) ;
}
