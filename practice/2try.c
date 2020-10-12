#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

typedef	struct		s_cmd
{
	char			**args;
	int				is_pipe;
	int				fd[2];
	struct	s_cmd	*prev;
	struct	s_cmd	*next;
}					t_cmd;

int		ft_strlen(char *s)
{
	int	i = 0;
	while (s[i])
		i++;
	return (i);
}

void	ft_putstr(char *s)
{
	write(2, s, ft_strlen(s));
}

void	exit_fatal();
{
	ft_putstr("error: fatal\n");
	exit(1);
}

int		ft_cd(t_cmd *cmd)
{
	int		res = 0;
	int		i = 0;
	while (cmd->args[i])
		i++;
	if (i != 2)
	{
		ft_putstr("error: cd: bad arguments\n");
		return (1);
	}
	else if ((res = chdir(cmd->argv[1])) < 0)
	{
		ft_putstr("error: cd: cannot change directory to ");
		ft_putstr(cmd->argv[1]);
		ft_putstr("\n");
	}
	return (res);
}

int		ft_non_builtin(t_cmd *cmd)
{
	extern	char 	**environ;
	pid_t			pid;
	int				res = 0;
	int				status;
	if (cmd->is_pipe)
		if ((pipe(cmd->fd[2])) < 0)
			exit_fatal();
	pid = fork();
	if (pid < 0)
		exit_fatal();
	else if (pid == 0)
	{
		if (cmd->is_pipe && dup2(cmd->fd[1], 1) < 0)
			exit_fatal();
		if (cmd->prev && cmd->prev->is_pipe && dup2(cmd->prev->fd[0], 0) < 0)
			exit_fatal();
		if ((res = execve(cmd->args[0], cmd->args, environ)) < 0)
		{
			ft_putstr("error: cannot execute ");
			ft_putstr(cmd->args[0]);
			ft_putstr("\n");
		}
		exit(res);
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			res = WEXITSTATUS(status);
		if (cmd->is_pipe)
		{
			close(cmd->fd[1]);
			if (!cmd->next)
				close(cmd->fd[0]);
		}
		if (cmd->prev && cmd->prev->is_pipe)
			close(cmd->prev->fd[0]);
	}
	return (res);
}

int		exec(t_cmd *cmd)
{
	int		res = 0;
	
	while (cmd)
	{
		if (!strcmp(cmd->argv[0], "cd"))
			res = ft_cd(cmd);
		else
			res = ft_non_builtin(cmd);
		cmd = cmd->next;
	}
	return (res);
}
