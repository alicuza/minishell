#include "minishell.h"

// TODO nik: how it works "exit | echo hello" why exit is ignored  is it because it is a part of pipeline?
// TODO nik: is it nessesary to closo any fd signal to childe process
int	builtin_exit(t_ctx *c, t_command_ctx *command_ctx)
{
	(void)c;
	(void)command_ctx;
	printf("exit\n");
	close(0);
	return (EXIT_SUCCESS);
}
