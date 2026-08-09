#include "minishell.h"

// static void	signal_handler(int signum, siginfo_t *info, void *ucontext)
// {
// 	(void) signum;
// 	(void) info;
// 	(void) ucontext;
// }

/*
by default non-interactive have job control disabled
When job control is not enabled, and Bash receives SIGINT while waiting for a foreground command, it waits until that foreground command terminates and then decides what to do about the SIGINT:

1 If the command terminates due to the SIGINT, Bash concludes that the user meant to send the SIGINT to the shell as well, and acts on the SIGINT (e.g., by running a SIGINT trap, exiting a non-interactive shell, or returning to the top level to read a new command).
2 If the command does not terminate due to SIGINT, the program handled the SIGINT itself and did not treat it as a fatal signal. In that case, Bash does not treat SIGINT as a fatal signal, either, instead assuming that the SIGINT was used as part of the program’s normal operation (e.g., emacs uses it to abort editing commands) or deliberately discarded. However, Bash will run any trap set on SIGINT, as it does with any other trapped signal it receives while it is waiting for the foreground command to complete, for compatibility.


by default for interactive shells  Bash attempts to enable job 
When job control is enabled, Bash does not receive keyboard-generated signals such as SIGINT while it is waiting for a foreground command. An interactive shell does not pay attention to the SIGINT, even if the foreground command terminates as a result, other than noting its exit status. If the shell is not interactive, and the foreground command terminates due to the SIGINT, Bash pretends it received the SIGINT itself (scenario 1 above), for compatibility.

see wtf is job control
//TODO nik: how to save disposition it had when the shell was started

todo handle the SIGPIPE to clear all
*/


/*
Handle ctrl-C, -- SIGINT
 ctrl-D -- Acts as an End-of-File (EOF) -- this should be handled by read line 
  and ctrl-\ which should behave like in bash. SIGQUIT

• In interactive mode:
◦ ctrl-C displays a new prompt on a new line.
◦ ctrl-D exits the shell.
◦ ctrl-\ does nothing.
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
