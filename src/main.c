/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lperroti <lperroti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 21:33:19 by lperroti          #+#    #+#             */
/*   Updated: 2023/03/21 19:28:38 by lperroti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

bool	check_cmd_in_paths(char *cmd_name, char *envp[])
{
	char	**paths;
	size_t	i;

	paths = get_paths(envp);
	if (!paths)
		return (false);
	i = 0;
	while (paths[i])
	{
		if (!lp_strcat(paths + i, cmd_name))
			break ;
		if (!access(paths[i], F_OK))
			return (free_tab(paths), true);
		i++;
	}
	return (free_tab(paths), false);
}

bool	check_cmds(int cmds_count, char *cmds_names[], char *envp[])
{
	char	*cmd_name;
	char	**tmp;

	while (cmds_count--)
	{
		tmp = lp_split(cmds_names[cmds_count], ' ');
		cmd_name = *tmp;
		if (!cmd_name || !*cmd_name)
			return (free_tab(tmp), false);
		if (access(cmd_name, F_OK) && !check_cmd_in_paths(cmd_name, envp))
			return (free_tab(tmp), false);
		free_tab(tmp);
	}
	return (true);
}

int	here_doc(char *limiter)
{
	char	*line;
	int		pipe_fds[2];
	int		cpid;

	new_pipe(pipe_fds);
	cpid = fork();
	if (!cpid)
	{
		close(pipe_fds[0]);
		line = get_next_line(0);
		while (lp_strncmp(line, limiter, lp_strlen(line) - 1))
		{
			lp_putendl_fd(line, pipe_fds[1]);
			free(line);
			line = get_next_line(0);
		}
		close(pipe_fds[1]);
		(free(line), exit(0));
	}
	else
	{
		close(pipe_fds[1]);
		wait(&cpid);
		return (pipe_fds[0]);
	}
	return (-1);
}

int	main(int argc, char *argv[], char *envp[])
{
	int		fd_in;
	int		fd_out;

	if ((argc-- && argv++) && argc < 4)
	{
		lp_putendl_fd((char *)"Use exemple: ./pipex file1 cmd1 cmd2 file2\n", 2);
		return (0);
	}
	if (!lp_strncmp(argv[0], "here_doc", 9) && argc-- && argv++)
		fd_in = here_doc(*(argv + 1));
	else
		fd_in = open(argv[0], O_RDONLY);
	if (fd_in == -1)
		exit(0);
	fd_out = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC);
	if (check_cmds(argc - 2, argv + 1, envp))
	{
		dup2(fd_in, STDIN_FILENO);
		dup2(fd_out, STDOUT_FILENO);
		pipe_while(argc - 2, argv + 1, envp);
	}
	else
		lp_putendl_fd("command not found", 1);
	(close(fd_in), close(fd_out));
	exit(EXIT_SUCCESS);
}
