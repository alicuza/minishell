#include "minishell.h"

// TODO nik: is it significant? 2.9.1.3 Commands with no Command Name
// If a simple command has no command name after word expansion (see 2.9.1.1 Order of Processing), any redirections shall be performed in a subshell environment; it is unspecified whether this subshell environment is the same one as that used for a command substitution within the command. (To affect the current execution environment, see the exec special built-in.) If any of the redirections performed in the current shell execution environment fail, the command shall immediately fail with an exit status greater than zero, and the shell shall write an error message indicating the failure. See 2.8.1 Consequences of Shell Errors for the consequences of these failures on interactive and non-interactive shells.

//A failure to open or create a file shall cause a redirection to fail.
/// . A redirection error causes the command to exit with a non-zero status.
void open_in_file(t_ctx *c, t_node *redir_node)
{
	char * filename;

	if(c->io_fd[0] != -1)
		close(c->io_fd[0]); //TODO look if failse
	errno = 0;
	filename = c->arena[AT_STRING].buf
			+ redir_node->data.redir.arena_offset; //TODO nik:The word following the redirection operator in the following descriptions, unless otherwise noted, is subjected to brace expansion, tilde expansion, parameter and variable expansion, command substitution, arithmetic expansion, quote removal, filename expansion, and word splitting.
	c->io_fd[0] = open(filename , O_RDONLY);// TODO process failure 
	if (c->io_fd[0] == -1)
	handle_error(filename, strerror(errno), 1);
}

void open_here_file(t_ctx *c, t_node *redir_node)
{
	(void) c;
	(void) redir_node;
	/* char * filename;

	if(c->io_fd[0] != -1)
		close(c->io_fd[0]); //TODO look if failse
	errno = 0;
	filename = c->arena[AT_STRING].buf
			+ redir_node->data.redir.arena_offset;
	c->io_fd[0] = open(filename , O_RDONLY);// TODO process failure 
	if (c->io_fd[0] == -1)
	handle_error(filename, strerror(errno), 1); */
}

void open_out_file(t_ctx *c, t_node *redir_node, int oflag)
{
	char * filename;

	if(c->io_fd[1] != -1)
		close(c->io_fd[1]); //TODO look if failse
	errno = 0;
	filename = c->arena[AT_STRING].buf
			+ redir_node->data.redir.arena_offset;
	c->io_fd[1] = open(filename, oflag, 0644);// TODO process failure 
	if (c->io_fd[1] == -1)
		handle_error(filename, strerror(errno), 1);
}

void process_redirection(t_ctx *c, t_node *redir_node)
{	
	while(redir_node != NULL)
	{
		if(redir_node.flags & REDIR_IN)
		{
			open_in_file(c, redir_node)
		}
		else if(redir_node.flags & REDIR_OUT)
		{
			open_out_file(c, redir_node, O_CREAT | O_WRONLY | O_TRUNC)
		}
		else if(redir_node.flags & REDIR_HERE)
			open_here_file(c, redir_node);
		else if(redir_node.flags & REDIR_APPEND)
		{
			open_out_file(c, redir_node, O_CREAT | O_WRONLY | O_APPEND)
		}
		redir_node = get_ptr_from_idx(&c->arena[AT_COMMAND], redir_node->data.redir.next);
	}
}
