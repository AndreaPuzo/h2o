#ifndef _H2O_H
# define _H2O_H

# define H2O_VERSION_MAJOR 0
# define H2O_VERSION_MINOR 0
# define H2O_VERSION_PATCH 0
# define H2O_VERSION            \
  (                             \
    (H2O_VERSION_MAJOR << 16) | \
    (H2O_VERSION_MINOR <<  8) | \
    (H2O_VERSION_PATCH <<  0)   \
  )

# define H2O_SUCCESS 0
# define H2O_FAILURE 1

int h2o (const char * input, const char * output) ;

#endif
