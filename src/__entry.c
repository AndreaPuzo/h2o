#include "h2o.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main (int argc, char ** argv)
{
  if (0 == argc)
    abort() ;
  
  if (1 == argc) {
    fprintf(stderr, "usage: %s [<option>...] <input>\n", argv[0]) ;
    fprintf(stderr, "usage: %s -h|--help\n", argv[0]) ;
    exit(EXIT_SUCCESS) ;
  }

  struct {
    char * input  ;
    char * output ;
  } opts ;

  opts.input  = NULL ;
  opts.output = NULL ;

  for (int i = 1 ; i < argc ; ++i) {
    if (0 == strcmp(argv[i], "-v") || 0 == strcmp(argv[i], "--ver")) {
      fprintf(stderr, "h2o %d.%d.%d\n", H2O_VERSION_MAJOR, H2O_VERSION_MINOR, H2O_VERSION_PATCH) ;
      exit(EXIT_SUCCESS) ;
    }
    
    if (0 == strcmp(argv[i], "-h") || 0 == strcmp(argv[i], "--help")) {
      fprintf(stderr, "usage: %s [<option>...] <input>\n", argv[0]) ;
      fprintf(stderr,
        "options:\n"
        "  -h, --help         --- Print this page.\n"
        "  -v, --ver          --- Print the version.\n"
        "  -o, --out <output> --- Set the output file.\n"
      ) ;
      exit(EXIT_SUCCESS) ;
    }
    
    if (0 == strcmp(argv[i], "-o") || 0 == strcmp(argv[i], "--out")) {
      if (++i == argc) {
        opts.output = argv[i] ;
      }
    } else {
      if (NULL != opts.input) {
        fprintf(stderr, "warning: ignored input `%s`\n", argv[i]) ;
      } else {
        opts.input = argv[i] ;
      }
    }
  }

  if (NULL == opts.input) {
    fprintf(stderr, "fatal: no input\n") ;
    fprintf(stderr, "usage: %s [<option>...] <input>\n", argv[0]) ;
    exit(EXIT_FAILURE) ;
  }

  char default_output [256 + 3] ;
  int i ;
  int c = 'a' ;
  
  for (i = 0 ; i < 256 ;) {
    if ('z' == c) {
      default_output[i] = c ;
      c = 'a' ;
      ++i ;
    } else {
      default_output[i] = c++ ;
    }
    
    default_output[i + 1] = '.' ;
    default_output[i + 2] = 'o' ;
    default_output[i + 3] = 0 ;
    
    FILE * fp = fopen(default_output, "r") ;

    if (NULL == fp)
      break ;
      
    fclose(fp) ;
  }

  if (NULL == opts.output) {
    opts.output = default_output ;
  }

  if (H2O_SUCCESS != h2o(opts.input, opts.output)) {
    fprintf(stderr, "Something has gone wrong...\n") ;
    exit(EXIT_FAILURE) ;
  }
  
  exit(EXIT_SUCCESS) ;
}
