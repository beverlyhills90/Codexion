#include "../codexion.h"

void *coders_routine(void *args)
{
    t_coder *coder = (t_coder *)args;
    while (*(coder->is_running) == 1)
    {
        take_dongle_wraper(coder);
        compile(coder->coder_id, coder->args->time_to_compile);
        giveup_dongle_wraper(coder);
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