#include "minishell.h"

// 	If the command is not found, the exit status shall be 127.
// Otherwise, if the command name is found, but it is not an executable utility, the exit status shall be 126.
// Otherwise, if the command terminated due to the receipt of a signal, the shell shall assign it an exit status greater than 128. The exit status shall identify, in an implementation-defined manner, which signal terminated the command. Note that shell implementations are permitted to assign an exit status greater than 255 if a command terminates due to a signal.
// Otherwise, the exit status shall be the value obtained by the equivalent of the WEXITSTATUS macro applied to the status obtained by the wait() function (as defined in the System Interfaces volume of POSIX.1-2024). Note that for C programs, this value is equal to the result of performing a modulo 256 operation on the value passed to _Exit(), _exit(), or exit() or returned from main().

static void	soft_exit_on_issue(char *error_prefix)
{
	perror(error_prefix);
	close(0);
}
void	wait_return_status(t_ctx *c)
{
	int wstatus;
	pid_t wpid;

#ifdef DEBUG
	fprintf(stderr, "\nchild pid=%jd\n", (intmax_t)c->pid_to_wait);
#endif
	wpid = waitpid(c->pid_to_wait, &wstatus, 0);
	while (wpid != -1 && !WIFEXITED(wstatus))
	{
		wpid = waitpid(c->pid_to_wait, &wstatus, 0);
	}
	if (wpid == -1)
	{
		c->return_status = EXIT_FAILURE;
		return (soft_exit_on_issue("waitpid"));
	}
	else
	{
#ifdef DEBUG
		printf("exited, status=%d\n", WEXITSTATUS(wstatus));
#endif
		c->return_status = WEXITSTATUS(wstatus);
		c->pid_to_wait = -1;
	}
}
