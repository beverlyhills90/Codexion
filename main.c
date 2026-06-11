#include "codexion.h"

int main(int argc, char **argv)
{
  t_argumnets *arguments;
  t_world_data *world_data;
  int i = 0;
  if (parsing_args(argv, argc, &arguments) != 0)
    return (1);
  printf("aaaaa:%d\n",arguments->dongle_cool_down);
  world_data_alocation(&world_data, arguments);
  coders_create(world_data->coders, world_data->args->number_of_coders);
  while (i < world_data->args->number_of_coders)
  {
    pthread_join(world_data->coders[i].thread_id, NULL);
    i++;
  }

  free(arguments);
  return (0);
}
