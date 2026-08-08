#include "minishell.h"

/*
In builtin export only mem issue can happen.
In cs:
 - If to many arg, log message, return Failure and continue the next one
 -If nessesary env var is missing, logs error return failure contine execution

In case mem issue, currently writs message and closes input for gracefull clean up at the end of main and return exit_failure for builtin
- todo maybe stop next comand in pipeline processing
- 

*/

// TODO nik: how we exit and make sure everethinh is closed
int	exit_mem_issue(void)
{
	perror("Memory allocation error");
	close(0);
	return (EXIT_FAILURE);
}

int handle_builtin_error(t_ctx *c, char *error_prefix, int error_code)
{
	perror(error_prefix);
	if(c->is_interactive == false)
		close(0);
	return (error_code);
}
