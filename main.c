#include "codexion.h"
// TODO suleep to nanosec *1000
int main(int argc, char **argv)
{
  t_argumnets *arguments;
  t_world_data *world_data;
  int i = 0;
  if (parsing_args(argv, argc, &arguments) != 0)
    return (1);
  world_data_alocation(&world_data, arguments);
  printf("n: %lu \n", world_data->args->time_to_burnout);
  int err = pthread_create(&world_data->thread_id, NULL, monitor, (void *)world_data);
  if (err != 0)
    return (fprintf(stderr, "monitor thread issue"), 1);
  coders_create(world_data->coders, world_data->args->number_of_coders);
  while (i < world_data->args->number_of_coders)
  {
    pthread_join(world_data->coders[i].thread_id, NULL);
    i++;
  }
  pthread_join(world_data->thread_id, NULL);
  free(arguments);
  return (0);
}
