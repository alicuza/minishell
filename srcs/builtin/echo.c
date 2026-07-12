#include "minishell.h"

int	echo(t_ctx *c, t_command_ctx *command_ctx) //TODO fix seg fault
{
  char	*str;
  char *tmp;
  
	str = command_ctx->argv[1];
	if (str[0] == '$')
	{
		tmp = env_get(&c->env, str + 1); 
    printf("%s\n", tmp);
    free(tmp);
    return 0;
	}
  printf("%s\n", str);
  return 0;
}