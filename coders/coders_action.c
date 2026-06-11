#include "../codexion.h"

void take_dongle(t_dongle *dongle, t_coder *coder)
{
    struct timespec ts;
    pthread_mutex_lock(&dongle->mutex);
    fifo_scheduler_add(dongle, coder);
    ts.tv_sec = dongle->cooldown / 1000;
    ts.tv_nsec = (dongle->cooldown % 1000) * 1000000;
    while (dongle->is_occupied || get_ms() < dongle->cooldown || dongle->queue[0] != coder)
    {
        pthread_cond_timedwait(&dongle->conditional, &dongle->mutex, &ts);
    }
    dongle->is_occupied = 1;
    fifo_scheduler_del(dongle);
    printf("%lu %lu has taken a dongle %i \n", get_ms(), coder->coder_id, dongle->dongle_id);
    pthread_mutex_unlock(&dongle->mutex);
}

void giveup_dongle(t_dongle *dongle, long long cooldown)
{
    pthread_mutex_lock(&dongle->mutex);
    dongle->cooldown = get_ms() + cooldown;
    dongle->is_occupied = 0;
    pthread_cond_broadcast(&dongle->conditional);
    pthread_mutex_unlock(&dongle->mutex);
}

void compile(int coder_id, long time_to_compile)
{
    printf("%lu %lu is compiling\n", get_ms(), coder_id);
    usleep(time_to_compile);
}

void take_dongle_wraper(t_coder *coder)
{
    if (coder->left < coder->right)
    {
        take_dongle(coder->right, coder);
        take_dongle(coder->left, coder);
    }
    else
    {
        take_dongle(coder->left, coder);
        take_dongle(coder->right, coder);
    }
}

void giveup_dongle_wraper(t_coder *coder)
{
    if (coder->left < coder->right)
    {
        giveup_dongle(coder->left, coder->args->dongle_cool_down);
        giveup_dongle(coder->right, coder->args->dongle_cool_down);
    }
    else
    {
        giveup_dongle(coder->right, coder->args->dongle_cool_down);
        giveup_dongle(coder->left, coder->args->dongle_cool_down);
    }
}