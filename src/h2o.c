#include "h2o.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

void calc_pos (char * data, long size, char * target, long * line, long * cpos)
{
  *line = 1 ;
  *cpos = 1 ;

  char * cp = data ;

  while (0 != *cp && target != cp) {
    if (0 != (isspace(*cp) && !isblank(*cp))) {
      ++*line ;
      *cpos = 1 ;
    } else
      ++*cpos ;
    
    ++cp ;
  }
}

int h2o (const char * input, const char * output)
{
  FILE * in ;
  FILE * out ;

#ifdef _WIN32
  in = fopen(input, "rb") ;
  out = fopen(output, "wb") ;
#else
  in = fopen(input, "r") ;
  out = fopen(output, "w") ;
#endif

  if (NULL == in) {
    fprintf(stderr, "error: cannot open the input `%s`\n", input) ;
    goto _error ;
  }

  if (NULL == out) {
    fprintf(stderr, "error: cannot create/open the output `%s`\n", output) ;
    goto _error ;
  }

  char * data ;
  long size ;

  fseek(in, 0, SEEK_END) ;
  size = ftell(in) ;
  fseek(in, 0, SEEK_SET) ;

  data = (char *)malloc(size + 1) ;

  if (NULL == data) {
    fprintf(stderr, "error: `malloc` failed allocating %llu bytes\n", size) ;
    goto _error ;
  }

  if (1 != fread(data, size, 1, in)) {
    fprintf(stderr, "error: cannot read the input data from `%s`\n", input) ;
    goto _error ;
  }

  data[size] = 0 ;

  char * cp = data ;
  long line ;
  long cpos ;

  while (0 != *cp) {
    while (0 != *cp && 0 != isspace(*cp)) {
      ++cp ;
    }
    
    if (0 == *cp)
      break ;
    
    if (';' == *cp) {
      ++cp ;

      while (0 != *cp && 0 == (isspace(*cp) && !isblank(*cp))) {
        ++cp ;
      }
    } else if ('\'' == *cp || '"' == *cp || '`' == *cp) {
      char quote = *cp++ ;

      while (0 != *cp && quote != *cp) {
        fputc(*cp++, out) ;
      }
      
      if (quote != *cp) {
        calc_pos(data, size, cp, &line, &cpos) ;
        fprintf(stderr, "In %s at %llu:%llu\n", input, line, cpos) ;
        fprintf(stderr, "error: unclosed string\n") ;
        free(data) ;
        goto _error ;
      }

      ++cp ;
    } else if ('.' == cp[0] && 'd' == cp[1] && 'u' == cp[2] && 'p' == cp[3]) {
      cp += 4 ;

      while (0 != isspace(*cp)) {
        ++cp ;
      }
      
      long count = 0 ;

      while (0 != isdigit(*cp)) {
        count *= 10 ;
        count += *cp - '0' ;
        ++cp ;
      }

      while (0 != isspace(*cp)) {
        ++cp ;
      }

      int v = 0 ;
      
      if ('0' <= *cp && *cp <= '9') {
        v += *cp - '0' ;
      } else if ('A' <= *cp && *cp <= 'F') {
        v += *cp - 'A' + 10 ;
      } else if ('a' <= *cp && *cp <= 'f') {
        v += *cp - 'a' + 10 ;
      } else {
        calc_pos(data, size, cp, &line, &cpos) ;
        fprintf(stderr, "In %s at %llu:%llu\n", input, line, cpos) ;
        fprintf(stderr, "error: invalid hex digit `%d` (`%c`)\n", *cp, isprint(*cp) ? *cp : '.') ;
        ++cp ; continue ;
      }
      
      ++cp ;
      
      v *= 16 ;
      
      if ('0' <= *cp && *cp <= '9') {
        v += *cp - '0' ;
      } else if ('A' <= *cp && *cp <= 'F') {
        v += *cp - 'A' + 10 ;
      } else if ('a' <= *cp && *cp <= 'f') {
        v += *cp - 'a' + 10 ;
      } else {
        calc_pos(data, size, cp, &line, &cpos) ;
        fprintf(stderr, "In %s at %llu:%llu\n", input, line, cpos) ;
        fprintf(stderr, "error: invalid hex digit `%d` (`%c`)\n", *cp, isprint(*cp) ? *cp : '.') ;
        ++cp ; continue ;
      }
      
      ++cp ;

      fprintf(stderr, "count: %llu\nchar: 0x%02X\n", count, v) ;

      while (0 < count) {
        fputc(v, out) ;
        --count ;
      }
    } else if (isxdigit(*cp)) {
      int v = 0 ;
      
      if ('0' <= *cp && *cp <= '9') {
        v += *cp - '0' ;
      } else if ('A' <= *cp && *cp <= 'F') {
        v += *cp - 'A' + 10 ;
      } else if ('a' <= *cp && *cp <= 'f') {
        v += *cp - 'a' + 10 ;
      } else {
        calc_pos(data, size, cp, &line, &cpos) ;
        fprintf(stderr, "In %s at %llu:%llu\n", input, line, cpos) ;
        fprintf(stderr, "error: invalid hex digit `%d` (`%c`)\n", *cp, isprint(*cp) ? *cp : '.') ;
        ++cp ; continue ;
      }
      
      ++cp ;
      
      v *= 16 ;
      
      if ('0' <= *cp && *cp <= '9') {
        v += *cp - '0' ;
      } else if ('A' <= *cp && *cp <= 'F') {
        v += *cp - 'A' + 10 ;
      } else if ('a' <= *cp && *cp <= 'f') {
        v += *cp - 'a' + 10 ;
      } else {
        calc_pos(data, size, cp, &line, &cpos) ;
        fprintf(stderr, "In %s at %llu:%llu\n", input, line, cpos) ;
        fprintf(stderr, "error: invalid hex digit `%d` (`%c`)\n", *cp, isprint(*cp) ? *cp : '.') ;
        ++cp ; continue ;
      }
      
      ++cp ;
      
      fputc(v, out) ;
    } else {
      calc_pos(data, size, cp, &line, &cpos) ;
      fprintf(stderr, "In %s at %llu:%llu\n", input, line, cpos) ;
      fprintf(stderr, "warning: unknow character `%d` (`%c`)\n", *cp, isprint(*cp) ? *cp : '.') ;
      ++cp ;
    }
  }

  free(data) ;
  fclose(in) ;
  fclose(out) ;

  return H2O_SUCCESS ;

_error :

  if (NULL != in)
    fclose(in) ;

  if (NULL != out)
    fclose(out) ;

  return H2O_FAILURE ;
}
