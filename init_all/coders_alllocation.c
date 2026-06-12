#include "../codexion.h"

t_coder *coders_allocation(t_argumnets *args, t_dongle *dongles, t_world_data *world_data)
{
    int i;
    i = 0;
    t_coder *coders;
    coders = ft_calloc(args->number_of_coders, sizeof(t_coder));
    if (!coders)
        return (NULL);
    while (i < args->number_of_coders)
    {
        coders[i].args = args;
        coders[i].coder_id = i + 1;
        coders[i].number_of_compiles = 0;
        coders[i].time_from_last_compilation = get_ms();
        coders[i].left = &dongles[i];
        coders[i].right = &dongles[(i + 1) % args->number_of_coders];
        pthread_mutex_init(&coders[i].mutex, NULL);
        coders[i].world_data = world_data;
        coders[i].is_working = 0;
        i++;
    }
    return (coders);
}