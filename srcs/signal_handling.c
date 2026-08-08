static void	signal_handler(int signum, siginfo_t *info, void *ucontext)
{
	static volatile sig_atomic_t	client_pid = 0;

	(void)ucontext;
	if (getpid() == info->si_pid && client_pid != 0)
	{
		process_own_signal(&client_pid, signum);
	}
	else
	{
		if (client_pid == 0)
			client_pid = info->si_pid;
		else if (client_pid != info->si_pid)
		{
			kill(info->si_pid, SIGUSR1);
			return (ft_putendl_fd(ERROR_SIG_IGNORE, STDERR_FILENO));
		}
		g_signal = signum;
	}
}

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


void	setup_signal_handler(t_ctx *c)
{
	struct sigaction	sact;

	In all cases, Bash ignores SIGQUIT
	if(c->is_interactive)
	{
		it ignores SIGTERM
		catches and handles SIGINT
	}
	sact.sa_flags = 0;
	sact.sa_sigaction = &signal_handler;
	if (sigaddset(&sact.sa_mask, SIGTERM) == -1
		|| sigaddset(&sact.sa_mask, SIGQUIT) == -1
		|| sigaction(SIGINT, &sact, NULL) == -1 //TODO handle it 
		|| sigaction(SIGHUP, &sact, NULL) == -1) //TODO The shell exits by default upon receipt of a SIGHUP. Before exiting, an interactive shell resends the SIGHUP to all jobs, running or stopped. The shell sends SIGCONT to stopped jobs to ensure that they receive the SIGHUP (See Job Control, for more information about running and stopped jobs)
	{
		ft_putendl_fd("Error: failed to setup server.", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
	ft_bzero(server, sizeof(t_server));
}
