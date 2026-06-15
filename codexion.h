#ifndef CODEXION_H
#define CODEXION_H

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

typedef enum e_scheduler
{
	FIFO,
	EDF
} t_scheduler;

typedef enum e_state
{
	DONE,
	NOT_DONE
} t_state;

typedef enum e_running
{
	RUNNING,
	STOP
} t_running;

typedef struct s_argumenst
{
	size_t number_of_coders;
	size_t time_to_burnout;
	size_t time_to_compile;
	size_t time_to_debug;
	size_t time_to_refactor;
	size_t number_of_compiles_required;
	size_t dongle_cool_down;
	t_scheduler scheduler;

} t_argumnets;

typedef struct s_coder t_coder;
typedef struct world_data t_world_data;

typedef struct s_dongle
{
	int dongle_id;
	pthread_cond_t conditional;
	pthread_mutex_t mutex;
	int is_occupied;
	long long cooldown;
	t_coder *queue[2];
	int queue_size;
} t_dongle;

typedef struct s_coder
{
	pthread_mutex_t mutex;
	unsigned int coder_id;
	t_dongle *left;
	t_dongle *right;
	unsigned long number_of_compiles;
	long long time_from_last_compilation;
	pthread_t thread_id;
	t_argumnets *args;
	t_world_data *world_data;
	int is_working;
} t_coder;

typedef struct world_data
{
	pthread_mutex_t world_mutex;
	pthread_t thread_id;
	t_running is_runnung;
	t_coder *coders;
	t_dongle *dongles;
	t_argumnets *args;
	long long time_of_start;
} t_world_data;

// helper func
void	*ft_calloc(size_t nmemb, size_t size);
long long	get_ms(void);
void	*monitor(void *args);
t_running	safe_world_state(t_world_data *world_data);
void	safe_world_stop(t_world_data *world_data);
int	safe_burnout_cheak(t_coder *coder); // 0 - burned 1 - OK
t_state	safe_coder_state(t_coder *coder);
unsigned long	get_num_of_coders(t_world_data *world_data);

// schedulers
void	fifo_scheduler_add(t_dongle *dongle,
			t_coder *coder);
void	scheduler_del(t_dongle *dongle);
void	scheduler_add(t_argumnets *args, t_coder *coder,
			t_dongle *dongle);
void	edf_scheduler(t_dongle *dongle, t_coder *coder);

// parsing
int	parsing_args(char **argv, int argc,
	 t_argumnets **arguments);

t_dongle	*dongles_allocation(unsigned long number);
int	world_data_alocation(t_world_data **world_data,
		t_argumnets *args);
t_coder	*coders_allocation(t_argumnets *args,
		t_dongle *dongles, t_world_data *world_data);

// coders
int	coders_create(t_coder *coders, size_t num,t_world_data *world_data);

void	free_coders(t_coder *coders, unsigned int num);
void	free_all(t_world_data *world_data);
void	free_dongles(t_dongle **dongles, unsigned int num);
// actions
void	take_dongle_wraper(t_coder *coder);
void	giveup_dongle_wraper(t_coder *coder);
void	compile(t_coder *coder);
void	debug(t_coder *coder);
void	refractoring(t_coder *coder);

#endif
