#include "../codexion.h"

void *coders_routine(void *args)
{
    t_coder *coder = (t_coder *)args;
    printf("IM ALIVE %d\n", coder->coder_id);
    while (safe_world_state(coder->world_data) == RUNNING)
    {
        take_dongle_wraper(coder);
        if (safe_world_state(coder->world_data) == STOP)
        {
            giveup_dongle_wraper(coder);
            break;
        }
        compile(coder);
        giveup_dongle_wraper(coder);
        if (safe_world_state(coder->world_data) == STOP)
            break;
        debug(coder);
        if (safe_world_state(coder->world_data) == STOP)
            break;
        refractoring(coder);
    }
    return NULL;
}

int coders_create(t_coder *coders, long num)
{
    int i;
    int err;
    i = 0;
    while (i < num)
    {
        err = pthread_create(&coders[i].thread_id, NULL, coders_routine, &coders[i]);
        if (err != 0)
            return (fprintf(stderr, "thread issue"), 1);
        i++;
    }
    return (0);
}