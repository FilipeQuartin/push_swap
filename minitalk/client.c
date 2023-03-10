#include "minitalk.h"
void    message(void)
{
    write(1, "usage: ./client [server-pid] [message]\n", 39);
	exit(0);
}

int	send_bit(int pid, char *str)
{
	static char	*message = 0;
	static int	s_pid = 0;
	static int	bits = -1;

	if (str)
		message = ft_strdup(str);
	if (!message)
		error(0);
	if (pid)
		s_pid = pid;
	if (message[++bits / 8])
	{
		if (message[bits / 8] & (0x80 >> (bits % 8)))
		{
			if (kill(s_pid, SIGUSR2) == -1)
				error(message);
		}
		else if (kill(s_pid, SIGUSR1) == -1)
			error(message);
		return (0);
	}
	if (!send_null(s_pid, message))
		return (0);
	free(message);
	return (1);
}

void	handler_sigusr(int signum)
{
	int	end;

	end = 0;
	if (signum == SIGUSR1)
		end = send_bit(0, 0);
	else if (signum == SIGUSR2)
	{
		write(1, "An Error ocured!\n", 17);
		exit(EXIT_FAILURE);
	}
	if (end)
	{
		write(1, "client: operation successful.\n", 30);
		exit(EXIT_SUCCESS);
	}
}

int main(int argc, char **argv)
{
    if (argc != 3)
        message();
    signal(SIGUSR1, handler_sigusr);
	signal(SIGUSR2, handler_sigusr);
	send_bit(atoi(argv[1]), argv[2]);
	while (1)
		pause();
	return (0);
}