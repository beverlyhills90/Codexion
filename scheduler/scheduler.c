#include "../codexion.h"

void scheduler_add(t_argumnets *args, t_coder *coder, t_dongle *dongle)
{
    if (args->scheduler == FIFO)
    {
        fifo_scheduler_add(dongle, coder);
    }
    else
    {
        edf_scheduler(dongle, coder);
    }
}

void scheduler_del(t_coder *coder, t_dongle *dongle)
{
    dongle->queue[0] = dongle->queue[1];
    dongle->queue[1] = NULL;
    dongle->queue_size--;
}