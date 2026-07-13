#include "../codexion.h"

void	take_dongle(t_dongle *dongle, t_coder *coder)
{
	struct timespec	ts;

	pthread_mutex_lock(&dongle->mutex);
	scheduler_add(coder->args, coder, dongle);
	while (safe_world_state(coder->world_data) == RUNNING
		&& (dongle->is_occupied || get_ms() < dongle->cooldown
			|| dongle->queue[0] != coder))
	{
		ts.tv_sec = dongle->cooldown / 1000;
		ts.tv_nsec = ((dongle->cooldown) % 1000) * 1000000;
		pthread_cond_timedwait(&dongle->conditional, &dongle->mutex, &ts);
	}
	if (safe_world_state(coder->world_data) == STOP)
	{
		scheduler_del(dongle);
		pthread_mutex_unlock(&dongle->mutex);
		return ;
	}
	dongle->is_occupied = 1;
	scheduler_del(dongle);
	pthread_mutex_lock(&coder->world_data->log_mutex);
	printf("%lld %u has taken a dongle\n",
		get_ms() - coder->world_data->time_of_start, coder->coder_id);
	pthread_mutex_unlock(&coder->world_data->log_mutex);
	pthread_mutex_unlock(&dongle->mutex);
}

void	giveup_dongle(t_dongle *dongle, long long cooldown)
{
	pthread_mutex_lock(&dongle->mutex);
	dongle->cooldown = get_ms() + cooldown;
	dongle->is_occupied = 0;
	pthread_cond_broadcast(&dongle->conditional);
	pthread_mutex_unlock(&dongle->mutex);
}

void	take_dongle_wraper(t_coder *coder)
{
	if (coder->coder_id % 2 == 0)
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

void	giveup_dongle_wraper(t_coder *coder)
{
	if (coder->coder_id % 2 == 0)
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
