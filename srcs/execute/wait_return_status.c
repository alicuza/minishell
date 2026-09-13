#include "minishell.h"

int	wait_return_status(t_ctx *c)
{
	int		wstatus;
	pid_t	wpid;
	pid_t	pid_to_wait;
	int		result;

	result = EXIT_FAILURE;
	pid_to_wait = c->pid_to_wait;
	while (1)
	{
		wpid = waitpid(-1, &wstatus, 0);
		if (wpid == -1)
		{
			if (errno == EINTR)
				continue ;
			if (errno != ECHILD)
			{
				msh_error("waitpid", NULL, strerror(errno));
				return (result);
			}
			break ;
		}
		if (wpid == pid_to_wait)
		{
			if (WIFSIGNALED(wstatus))
				result = 128 + WTERMSIG(wstatus);
			else if (WIFEXITED(wstatus))
				result = WEXITSTATUS(wstatus);
		}
	}
	c->pid_to_wait = -1;
	g_signal = 0;
	return (result);
}
