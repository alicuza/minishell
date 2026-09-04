#include "minishell.h"

int	cleanup(t_ctx *c)
{
	arena_free(&c->arena[AT_STRING]);
	arena_free(&c->arena[AT_TOKENS]);
	arena_free(&c->arena[AT_STACK]);
	arena_free(&c->arena[AT_PROMPT]);
	arena_free(&c->arena[AT_COMMAND]);
	free_env(&c->env);
	ft_close_fd(&c->io_fd[0]);
	ft_close_fd(&c->io_fd[1]);
	ft_close_fd(&c->pipe_fd[0]);
	ft_close_fd(&c->pipe_fd[1]);
	close(0); //TODO will there be double close error
	close(1);
	close(2);
	return (0);
}

void	free_str_arr(char **val)
{
	int	i;

	i = 0;
	while (val[i] != NULL)
	{
		free(val[i]);
		i++;
	}
	free(val);
}
