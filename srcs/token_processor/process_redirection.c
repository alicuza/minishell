#include "minishell.h"

int open_in_file(t_ctx *c, t_node *redir_node)
{
	char * filename;

	ft_close_fd(&c->io_fd[0]);
	errno = 0;
	filename = c->arena[AT_STRING].buf
			+ redir_node->data.redir.arena_offset; //TODO nik:The word following the redirection operator in the following descriptions, unless otherwise noted, is subjected to variable expansion, quote removal, (optional )filename expansion, and word splitting.
	c->io_fd[0] = open(filename , O_RDONLY);
	if (c->io_fd[0] == -1)
		return handle_redirection_error(c, filename, EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int open_here_file(t_ctx *c, t_node *redir_node)
{
	c->io_fd[0] = redir_node->data.redir.fd;
	if (c->io_fd[0] == -1)
		return handle_redirection_error(c, "here-doc", EXIT_FAILURE);

	return (EXIT_SUCCESS);
}

int open_out_file(t_ctx *c, t_node *redir_node, int oflag)
{
	char * filename;

	ft_close_fd(&c->io_fd[1]);
	errno = 0;
	filename = c->arena[AT_STRING].buf
			+ redir_node->data.redir.arena_offset;
	c->io_fd[1] = open(filename, oflag, 0644);
	if (c->io_fd[1] == -1)
		return handle_redirection_error(c, filename, EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int open_based_on_flag(t_ctx *c, t_node *redir_node)
{
	if(redir_node->flags & REDIR_IN)
	{
		if(open_in_file(c, redir_node) == EXIT_FAILURE)
			return EXIT_FAILURE;
	}
	else if(redir_node->flags & REDIR_OUT)
	{
		if(open_out_file(c, redir_node, O_CREAT | O_WRONLY | O_TRUNC) == EXIT_FAILURE)
			return EXIT_FAILURE;
	}
	else if(redir_node->flags & REDIR_HERE)
	{
		if(open_here_file(c, redir_node) == EXIT_FAILURE)
			return EXIT_FAILURE;
	}
	else if(redir_node->flags & REDIR_APPEND)
	{
		if(open_out_file(c, redir_node, O_CREAT | O_WRONLY | O_APPEND) == EXIT_FAILURE)
			return EXIT_FAILURE;
	}
		return EXIT_SUCCESS;
}

int process_redirection(t_ctx *c, t_node *redir_node)
{	
	while(redir_node->type == NODE_REDIR)
	{
		if(open_based_on_flag(c, redir_node) == EXIT_FAILURE)
			return EXIT_FAILURE;
		redir_node = get_ptr_from_idx(&c->arena[AT_COMMAND], redir_node->next_idx);
	}
		return (EXIT_SUCCESS);
}
