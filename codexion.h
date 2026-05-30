#ifndef CODEXION_H
#define CODEXION_H

#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct s_argumenst
{
  unsigned long number_of_coders;
  unsigned long time_to_burnout;
  unsigned long time_to_compile;
  unsigned long time_to_debug;
  unsigned long time_to_refactor;
  unsigned long number_of_compiles_required;
  unsigned long dongle_cool_down;
  int scheduler; // 0 - fifo 1 - edf

} t_argumnets;
// helper func
void *ft_calloc(size_t nmemb, size_t size);

int parsing_args(char **argv, int argc, t_argumnets **arguments);

#endif
