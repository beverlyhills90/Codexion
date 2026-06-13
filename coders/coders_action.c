#include "../codexion.h"

void	compile(t_coder *coder)
{
	long long	log_time;

	if (safe_world_state(coder->world_data) == STOP)
		return ;
	pthread_mutex_lock(&coder->mutex);
	coder->is_working = 1;
	log_time = get_ms() - coder->world_data->time_of_start;
	printf("%lld %u coder is compiling\n", log_time, coder->coder_id);
	coder->time_from_last_compilation = get_ms();
	coder->number_of_compiles += 1;
	pthread_mutex_unlock(&coder->mutex);
	usleep(coder->args->time_to_compile * 1000);
}
void	debug(t_coder *coder)
{
	long long	log_time;

	log_time = get_ms() - coder->world_data->time_of_start;
	printf("%lld %u is debuging\n", log_time, coder->coder_id);
	usleep(coder->world_data->args->time_to_debug * 1000);
}
void	refractoring(t_coder *coder)
{
	long long log_time;
	log_time = get_ms() - coder->world_data->time_of_start;
	printf("%lld %u is refractoring\n", log_time, coder->coder_id);
	usleep(coder->world_data->args->time_to_refactor * 1000);
}