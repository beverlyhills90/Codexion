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
void fifo_scheduler_del(t_dongle *dongle)
{
    dongle->queue[0] = dongle->queue[1];
    dongle->queue[1] = NULL;
    dongle->queue_size--;
}