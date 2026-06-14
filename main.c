#include "codexion.h"

int	join_coders(t_world_data *world_data)
{
	unsigned long	i;
	int				err;

	i = 0;
	while (i < world_data->args->number_of_coders)
	{
		err = pthread_join(world_data->coders[i].thread_id, NULL);
		if (err != 0)
		{
			free_all(world_data);
			return (1);
		}
		i++;
	}
	return (0);
}

int	join_all(t_world_data *world_data, t_argumnets *arguments)
{
	int	err;

	err = pthread_create(&world_data->thread_id, NULL, monitor,
			(void *)world_data);
	if (err != 0)
		return (free_all(world_data), free(arguments), 1);
	if (coders_create(world_data->coders,
			world_data->args->number_of_coders) != 0)
		return (free_all(world_data), free(arguments), 1);
	if (join_coders(world_data) != 0)
		return (free(arguments), 1);
	if (pthread_join(world_data->thread_id, NULL) != 0)
		return (free_all(world_data), 1);
	return (0);
}

int	main(int argc, char **argv)
{
	t_argumnets		*arguments;
	t_world_data	*world_data;
	int				err;

	if (parsing_args(argv, argc, &arguments) != 0)
		return (free(arguments), 1);
	if (world_data_alocation(&world_data, arguments) != 0)
		return (free(arguments), 1);
	err = join_all(world_data, arguments);
	if (err != 0)
		return (1);
	free_all(world_data);
	free(arguments);
	return (0);
}
