#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

#include <stdio.h>

typedef struct		s_cmd
{
	char			**args;
	int				is_pipe;
	int				fd[2];
	struct s_cmd	*prev;
	struct s_cmd	*next;
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

void	exit_fatal()
{
	ft_putstr("error: fatal\n");
	exit(1);
}

char	*ft_strdup(char *s)
{
	int 	i = 0;
	char	*ptr;
	if (!(ptr = malloc(sizeof(char) * (ft_strlen(s) + 1))))
		exit_fatal();
	while (s[i])
	{
		ptr[i] = s[i];
		i++;
	}
	ptr[i] = 0;
	return (ptr);
}

void	clear(t_cmd *cmd)
{
	t_cmd	*tmp;
	int		i = 0;
	while (cmd)
	{
		while (cmd->args[i])
		{
			free(cmd->args[i]);
			i++;
		}
		free(cmd->args);
		tmp = cmd;
		cmd = cmd->next;
		free(tmp);
	}
}

t_cmd	*create_cmd(t_cmd *tmp, char **argv, int argv_num, int is_pipe)
{
	t_cmd	*new;
	int		i = 0;
	if (!(new = malloc(sizeof(t_cmd))))
		exit_fatal();
	if (!(new->args = malloc(sizeof(char *) * (argv_num + 1))))
		exit_fatal();
	while (i < argv_num)
	{
		new->args[i] = ft_strdup(argv[i]);
		i++;
	}
	new->args[i] = NULL;
	new->is_pipe = is_pipe;
	new->prev = tmp;
	new->next = NULL;
	if (tmp)
		tmp->next = new;
	return (new);
}

int	ft_cd(t_cmd *cmd)
{
	int		i = 0;
	int		res = 0;
	while (cmd->args[i])
		i++;
	if (i != 2)
	{
		ft_putstr("error: cd: bad arguments\n");
		return (1);
	}
	else if ((res = chdir(cmd->args[1])) < 0)
	{
		ft_putstr("error: cd: cannot change directory to ");
		ft_putstr(cmd->args[1]);
		ft_putstr("\n");
	}
	printf("res = %d\n", res);
	return (res);
}

int	exec(t_cmd *cmd)
{
	int		res = 0;
	
	while (cmd)
	{
		if (!strcmp(cmd->args[0], "cd"))
			res = ft_cd(cmd);
		//else
		//	res = ft_non_builtin(cmd);
		cmd = cmd->next;
	}
	return (res);
}

int	main(int argc, char **argv)
{
	t_cmd	*cmd;
	t_cmd	*tmp;
	int		start = 1;
	int		last = 1;
	int		is_pipe = 0;
	int		res = 0;
	
	while (last < argc)
	{
		if (!strcmp(argv[last], "|") || !strcmp(argv[last], ";") || last + 1 == argc)
		{
			if (!strcmp(argv[last], "|"))
				is_pipe = 1;
			else if (!strcmp(argv[last], ";"))
				is_pipe = 0;
			else
			{
				is_pipe = 0;
				last++;
			}
			if (last - start != 0)
			{
				tmp = create_cmd(tmp, &argv[start], last - start, is_pipe);
				if (!cmd)
					cmd = tmp;
			}
			start = last + 1;
		}
		last++;
	}
	res = exec(cmd);
	printf("res = %d\n", res);
	clear(cmd);
	return (res);
}
