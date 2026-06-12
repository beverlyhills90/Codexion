#include "../codexion.h"

int world_data_alocation(t_world_data **world_data, t_argumnets *args)
{
    *world_data = ft_calloc(1, sizeof(t_world_data));
    if (!*world_data)
        return (fprintf(stderr, "somthing went wrong go to gdb..."), 1);
    (*world_data)->dongles = dongles_allocation(args->number_of_coders);
    if (!(*world_data)->dongles)
        return (1); // TODO free all
    (*world_data)->time_of_start = get_ms();
    pthread_mutex_init(&(*world_data)->world_mutex, NULL);
    (*world_data)->args = args;
    (*world_data)->coders = coders_allocation(args, (*world_data)->dongles, (*world_data));
    if (!(*world_data)->coders)
        return (1); // TODO free all
    (*world_data)->is_runnung = RUNNING;
    return (0);
}