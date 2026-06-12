#include "../codexion.h"

void *ft_calloc(size_t nmemb, size_t size)
{
	void *res;

	if (nmemb == 0 || size == 0)
		return (malloc(1));
	if (nmemb != 0 && size > (size_t)-1 / nmemb)
		return (NULL);
	res = malloc(nmemb * size);
	if (!res)
		return (NULL);
	memset(res, '\0', nmemb * size);
	return (res);
}

long long get_ms()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);

	long long milliseconds = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	// if need milisec of this sec tv.tv_sec /1000
	return milliseconds;
}

t_running safe_world_state(t_world_data *world_data)
{
	t_running res;
	pthread_mutex_lock(&world_data->world_mutex);
	if (world_data->is_runnung == RUNNING)
		res = RUNNING;
	else
		res = STOP;
	pthread_mutex_unlock(&world_data->world_mutex);
	return (res);
}
void safe_world_stop(t_world_data *world_data)
{
	size_t i;
	i = 0;
	t_dongle *current_dongle;
	pthread_mutex_lock(&world_data->world_mutex);
	world_data->is_runnung = STOP;
	while (i < world_data->args->number_of_coders)
	{
		pthread_cond_broadcast(&world_data->dongles[i].conditional);
		i++;
	}
	pthread_mutex_unlock(&world_data->world_mutex);
}

int safe_burnout_cheak(t_coder *coder)
{
	long long time_to;
	int res;
	pthread_mutex_lock(&coder->mutex);
	time_to = coder->time_from_last_compilation + coder->args->time_to_burnout;
	if (get_ms() >= time_to)
		res = 0;
	else
		res = 1;
	pthread_mutex_unlock(&coder->mutex);
	return (res);
}
