#include "../codexion.h"

void free_dongles(t_dongle **dongles, unsigned int num)
{
	size_t i;

	i = 0;
	while (i < num)
	{
		pthread_cond_destroy(&((*dongles)[i]).conditional);
		pthread_mutex_destroy(&((*dongles)[i]).mutex);
		i++;
	}
	free(*dongles);
}

void free_coders(t_coder *coders, unsigned int num)
{
	size_t i;

	i = 0;
	while (i < num)
	{
		pthread_mutex_destroy(&coders[i].mutex);
		i++;
	}
	free(coders);
}

void free_all(t_world_data *world_data)
{
	free_dongles(&world_data->dongles, world_data->args->number_of_coders);
	free_coders(world_data->coders, world_data->args->number_of_coders);
	pthread_mutex_destroy(&world_data->world_mutex);
	free(world_data);
}