#include "../codexion.h"

void compile(t_coder *coder)
{
    long long log_time;

    pthread_mutex_lock(&coder->mutex);
    coder->is_working = 1;
    log_time = get_ms() - coder->world_data->time_of_start;

    printf("%lu %lu coder is compiling\n", log_time, coder->coder_id);
    coder->time_from_last_compilation = get_ms();
    pthread_mutex_unlock(&coder->mutex);

    usleep(coder->args->time_to_compile);

    pthread_mutex_lock(&coder->mutex);
    coder->number_of_compiles += 1;
    coder->time_from_last_compilation = get_ms();
    pthread_mutex_unlock(&coder->mutex);
}
void debug(t_coder *coder)
{
    long long log_time;
    log_time = get_ms() - coder->world_data->time_of_start;

    usleep(coder->world_data->args->time_to_debug);
    printf("%lu %lu is debuging\n", log_time, coder->coder_id);
}
void refractoring(t_coder *coder)
{
    long long log_time;
    log_time = get_ms() - coder->world_data->time_of_start;

    usleep(coder->world_data->args->time_to_refactor);
    printf("%lu %lu is refractoring\n", log_time, coder->coder_id);
}