/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lperroti <lperroti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 21:33:19 by lperroti          #+#    #+#             */
/*   Updated: 2023/03/15 07:23:37 by lperroti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	free_tab(char **tab)
{
	char	**tmp;

	tmp = tab;
	while (*tab)
	{
		free(*tab);
		tab++;
	}
	free(tmp);
}

t_fork_cmd_params	init_params(char *envp[])
{
	t_fork_cmd_params	params;

	params.envp = envp;
	params.dup_fds = (int **)malloc(2 * sizeof(int *));
	params.dup_fds[0] = (int *)malloc(2 * sizeof(int));
	params.dup_fds[1] = (int *)malloc(2 * sizeof(int));

	return (params);
}

void	new_pipe(int pipe_fds[2])
{
	if (pipe(pipe_fds) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
}

void	reinit_params(t_fork_cmd_params *params, char *cmd_list[],
	int pipe_fds[2], size_t i, size_t cmd_list_count)
{
	lp_bzero(params->dup_fds[0], 2 * sizeof(int));
	lp_bzero(params->dup_fds[1], 2 * sizeof(int));
	params->cmd_args = lp_split(cmd_list[i], ' ');
	params->cmd_name = *params->cmd_args;
	params->dup_fds_count = 1;
	params->dup_fds[0][0] = pipe_fds[0];
	params->dup_fds[0][1] = STDIN_FILENO;
	params->close_fd = pipe_fds[0];
	if (!i)
	{
		params->dup_fds[0][0] = pipe_fds[1];
		params->dup_fds[0][1] = STDOUT_FILENO;
	}
	else if (i + 1 == cmd_list_count)
		params->close_fd = pipe_fds[1];
	else
	{
		close(pipe_fds[1]);
		new_pipe(pipe_fds);
		params->dup_fds[1][0] = pipe_fds[1];
		params->dup_fds[1][1] = STDOUT_FILENO;
		params->dup_fds_count = 2;
	}
}

void	pipe_while(int cmd_list_count, char *cmd_list[], char *envp[])
{
	int					pipe_fds[2];
	t_fork_cmd_params	params;
	int					i;

	params = init_params(envp);

	new_pipe(pipe_fds);
	i = 0;
	while (i < cmd_list_count)
	{
		reinit_params(&params, cmd_list, pipe_fds, i, cmd_list_count);
		fork_exec_cmd(params);
		free_tab(params.cmd_args);
		i++;
	}
	(free(params.dup_fds[0]), free(params.dup_fds[1]), free(params.dup_fds));
}

int	main(int argc, char *argv[], char *envp[])
{
	int		fd_in;
	int		fd_out;

	if (argc < 5)
	{
		lp_putendl_fd((char *)"Use exemple: ./pipex file1 cmd1 cmd2 file2\n", 2);
		return (0);
	}
	fd_in = open(argv[1], O_RDONLY);
	fd_out = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC);
	dup2(fd_in, STDIN_FILENO);
	dup2(fd_out, STDOUT_FILENO);
	pipe_while(argc - 3, argv + 2, envp);
	exit(EXIT_SUCCESS);
}
