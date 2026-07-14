#include "minishell.h"

int	builtin_exit(t_ctx *c, t_command_ctx *command_ctx)
{
  (void) c;
  (void) command_ctx;
  printf("exit\n");
  close(0);
  return EXIT_SUCCESS;
}
