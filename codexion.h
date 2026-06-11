#ifndef CODEXION_H
#define CODEXION_H

#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

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

typedef struct s_coder t_coder;

typedef struct s_dongle
{
  pthread_mutex_t mutex;
  int dongle_id;
  pthread_cond_t conditional;
  int is_occupied;
  long long cooldown;
  t_coder *queue[2];
  int queue_size;
} t_dongle;

typedef struct s_coder
{
  int coder_id;
  t_dongle *left;
  t_dongle *right;
  unsigned long number_of_compiles;
  long long time_from_last_compilation;
  pthread_t thread_id;
  t_argumnets *args;
  int *is_running;
} t_coder;

typedef struct world_data
{
  pthread_mutex_t world_mutex;
  int is_runnung; // 0 - not 1 - running
  long long time_of_start;
  t_coder *coders;
  t_dongle *dongles;
  t_argumnets *args;
} t_world_data;

// helper func
void *ft_calloc(size_t nmemb, size_t size);
long long get_ms();

void fifo_scheduler_del(t_dongle *dongle);
void fifo_scheduler_add(t_dongle *dongle, t_coder *coder);

// parsing
int parsing_args(char **argv, int argc, t_argumnets **arguments);

t_dongle *dongles_allocation(unsigned long number);
int world_data_alocation(t_world_data **world_data, t_argumnets *args);
t_coder *coders_allocation(t_argumnets *args, t_dongle *dongles, int *is_runnung);

// coders
int coders_create(t_coder *coders, long num);

// actions
void take_dongle_wraper(t_coder *coder);
void giveup_dongle_wraper(t_coder *coder);
void compile(int coder_id, long time_to_compile);
#endif
