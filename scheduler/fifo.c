#include "../codexion.h"

void fifo_scheduler_add(t_dongle *dongle, t_coder *coder)
{
    int i;
    i = 0;
    while (dongle->queue[i])
        i++;
    dongle->queue[i] = coder;
    dongle->queue_size++;
}
