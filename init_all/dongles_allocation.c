#include "../codexion.h"

t_dongle *dongles_allocation(unsigned long number)
{
    int i;
    i = 0;
    t_dongle *dongles;

    dongles = ft_calloc(number, sizeof(t_dongle));
    if (!dongles)
        return (NULL);
    while (i < number)
    {
        dongles[i].cooldown = 0;
        dongles[i].is_occupied = 0;
        dongles[i].dongle_id = i;
        pthread_mutex_init(&dongles[i].mutex, NULL);
        pthread_cond_init(&dongles[i].conditional, NULL);
        dongles[i].queue_size = 0;
        i++;
    }
    return (dongles);
}