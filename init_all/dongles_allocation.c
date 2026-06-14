#include "../codexion.h"

t_dongle	*dongles_allocation(unsigned long number)
{
	size_t		i;
	t_dongle	*dongles;

	i = 0;
	dongles = ft_calloc(number, sizeof(t_dongle));
	if (!dongles)
		return (NULL);
	while (i < number)
	{
		dongles[i].cooldown = 0;
		dongles[i].is_occupied = 0;
		if(pthread_cond_init(&dongles[i].conditional, NULL) != 0)
            return(free_dongles(&dongles,i),NULL);
		if(pthread_mutex_init(&dongles[i].mutex, NULL) != 0)
			return(free_dongles(&dongles,i),NULL);
		dongles[i].queue_size = 0;
		i++;
	}
	return (dongles);
}
