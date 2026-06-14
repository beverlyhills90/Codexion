#include "../codexion.h"

t_state	safe_coder_state(t_coder *coder)
{
	pthread_mutex_lock(&coder->mutex);
	if (coder->number_of_compiles >= coder->args->number_of_compiles_required)
	{
		pthread_mutex_unlock(&coder->mutex);
		return (DONE);
	}
	pthread_mutex_unlock(&coder->mutex);
	return (NOT_DONE);
}

unsigned long	get_num_of_coders(t_world_data *world_data)
{
	unsigned long	res;

	pthread_mutex_lock(&world_data->world_mutex);
	res = world_data->args->number_of_coders;
	pthread_mutex_unlock(&world_data->world_mutex);
	return (res);
}

void	*monitor(void *args)
{
	t_world_data	*world_data;
	size_t			i;
	t_coder			*current_coder;
	unsigned int	done;

	i = 0;
	done = 0;
	world_data = (t_world_data *)args;
	while (1)
	{
		done = 0;
		if (safe_world_state(world_data) == STOP)
			return (safe_world_stop(world_data), NULL);
		i = 0;
		while (i < world_data->args->number_of_coders)
		{
			current_coder = &world_data->coders[i];
			if (safe_burnout_cheak(current_coder) != 0)
			{
				printf("%lld %d coder is burned out\n", get_ms()
					- world_data->time_of_start, current_coder->coder_id);
				safe_world_stop(world_data);
				return (NULL);
			}
			if (safe_coder_state(current_coder) == DONE)
				done++;
			i++;
		}
		if (get_num_of_coders(world_data) == done)
		{
			safe_world_stop(world_data);
			return (NULL);
		}
		usleep(500);
	}
}
