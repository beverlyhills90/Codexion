#include "../codexion.h"

void *monitor(void *args)
{
    t_world_data *world_data;
    size_t i;
    t_coder *current_coder;
    unsigned int done;
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
            if (safe_burnout_cheak(current_coder) == 0)
            {
                pthread_mutex_lock(&current_coder->mutex);
                printf("%d coder is burned out\n", current_coder->coder_id);

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
        usleep(2000);
    }
}
