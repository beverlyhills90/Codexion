#include "codexion.h"

int main(int argc, char **argv)
{
  t_argumnets *arguments;

  printf("%d\n", argc);

  parsing_args(argv, argc, &arguments);
  printf("%lu\n", arguments->dongle_cool_down);
  free(arguments);
  return (0);
}
