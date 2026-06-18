#include "minishell.h"

int	builtin_exit(t_ctx *c)
{
  (void) c;
  printf("exit\n");
  close(0);
  return 0;
}
