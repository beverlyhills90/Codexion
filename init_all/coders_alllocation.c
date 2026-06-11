#include "../codexion.h"

t_coder *coders_allocation(t_argumnets *args, t_dongle *dongles, int *is_running)
{
    int i;
    i = 0;
    t_coder *coders;
    coders = ft_calloc(args->number_of_coders, sizeof(t_coder));
    if (!coders)
        return (NULL);
    while (i <= args->number_of_coders)
    {
        coders[i].args = args;
        coders[i].coder_id = i + 1;
        coders[i].number_of_compiles = 0;
        coders[i].time_from_last_compilation = 0;
        coders[i].left = &dongles[i];
        coders[i].right = &dongles[(i + 1) % args->number_of_coders];
        coders[i].is_running = is_running;
        i++;
    }
    return (coders);
}