#include "minishell.h"

int	builtin_exit(t_ctx *c, char **argv)
{
  (void) c;
  (void) argv;
  printf("exit\n");
  close(0);
  return 0;
}
