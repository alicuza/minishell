#include "minishell.h"

// static void	signal_handler(int signum, siginfo_t *info, void *ucontext)
// {
// 	(void) signum;
// 	(void) info;
// 	(void) ucontext;
// }

/*

*/

/*
TODO nik:
• In interactive mode:
◦ ctrl-C displays a new prompt on a new line. SIGINT
◦ ctrl-\ does nothing. ignore  SIGQUIT
	todo ignore SIGPIPE, how to unset all the defaults / ingnore all
*/
int	setup_signal_handler(t_ctx *c)
{
	// struct sigaction	sact_to_ignore;
	// struct sigaction	sact_to_hanle;

	(void) c; //Use to determine if it is interactive
	// sact_to_ignore.sa_handler = SIG_IGN;
	// sact_to_hanle.sa_flags = SA_SIGINFO | SA_RESTART;
	// sact_to_hanle.sa_sigaction = &signal_handler;
	// if ( sigemptyset(&sact_to_ignore.sa_mask) == -1
	// 	|| sigemptyset(&sact_to_hanle.sa_mask) == -1
	// 	|| sigaction(SIGQUIT, &sact_to_ignore, NULL) == -1 // In all cases, Bash ignores SIGQUIT
	// 	|| sigaction(SIGTERM, &sact_to_ignore, NULL) == -1 //	If Bash is interactive
	// 	|| sigaction(SIGINT, &sact_to_hanle, NULL) == -1 // If Bash is interactive
	// 	//  If job control is in effect (see Job Control), Bash ignores SIGTTIN, SIGTTOU, and SIGTSTP.
	// 	|| sigaction(SIGHUP, &sact_to_hanle, NULL) == -1) //TODO The shell exits by default upon receipt of a SIGHUP. Before exiting, an interactive shell resends the SIGHUP to all jobs, running or stopped. The shell sends SIGCONT to stopped jobs to ensure that they receive the SIGHUP (See Job Control, for more information about running and stopped jobs)
	// {
	// 	ft_putendl_fd("Error: failed to setup signal handdler", STDERR_FILENO);
	// 	close(0);
	// 	return (EXIT_FAILURE);
	// }
	return (EXIT_SUCCESS);
}
