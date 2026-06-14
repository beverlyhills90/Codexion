#include "../codexion.h"

void	edf_scheduler(t_dongle *dongle, t_coder *coder)
{
	t_coder		*coder_1;
	long long	timing_1;
	long long	timing_2;

	if (dongle->queue_size == 0)
		dongle->queue[0] = coder;
	else
	{
		coder_1 = dongle->queue[0];
		timing_1 = coder_1->time_from_last_compilation
			+ coder_1->args->time_to_burnout;
		timing_2 = coder_1->time_from_last_compilation
			+ coder->args->time_to_burnout;
		if (timing_1 > timing_2)
		{
			dongle->queue[1] = coder;
		}
		else
		{
			dongle->queue[0] = coder;
			dongle->queue[1] = coder_1;
		}
	}
	dongle->queue_size += 1;
}
