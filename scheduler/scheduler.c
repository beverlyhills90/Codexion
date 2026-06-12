#include "../codexion.h"

void scheduler_add(t_argumnets *args, t_coder *coder, t_dongle *dongle)
{
    if (args->scheduler == FIFO)
    {
        fifo_scheduler_add(dongle, coder);
    }
}

void scheduler_del(t_argumnets *args, t_coder *coder, t_dongle *dongle)
{
    if (args->scheduler == FIFO)
    {
        fifo_scheduler_del(dongle);
    }
}