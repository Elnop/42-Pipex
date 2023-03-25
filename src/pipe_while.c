/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_while.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lperroti <lperroti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/18 02:48:43 by lperroti          #+#    #+#             */
/*   Updated: 2023/03/25 21:41:21 by lperroti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

t_fork_cmd_params	init_params(char *envp[])
{
	t_fork_cmd_params	params;

	params.envp = envp;
	params.dup_fds = (int **)malloc(2 * sizeof(int *));
	params.dup_fds[0] = (int *)malloc(2 * sizeof(int));
	params.dup_fds[1] = (int *)malloc(2 * sizeof(int));
	return (params);
}

void	reset_params(t_fork_cmd_params *params, char *cmd_list[],
	int pipe_fds[2], size_t i)
{
	lp_bzero(params->dup_fds[0], 2 * sizeof(int));
	lp_bzero(params->dup_fds[1], 2 * sizeof(int));
	params->cmd_args = lp_split(cmd_list[i], ' ');
	params->cmd_name = *params->cmd_args;
	params->dup_fds_count = 1;
	params->dup_fds[0][0] = pipe_fds[0];
	params->dup_fds[0][1] = STDIN_FILENO;
	params->close_fd = pipe_fds[0];
}

void	conditional_params(t_fork_cmd_params *params,
	int pipe_fds[2], size_t i, size_t cmd_list_count)
{
	if (!i)
	{
		params->dup_fds[0][0] = pipe_fds[1];
		params->dup_fds[0][1] = STDOUT_FILENO;
	}
	else if (i + 1 == cmd_list_count)
	{
		close(pipe_fds[1]);
		params->close_fd = pipe_fds[1];
	}
	else
	{
		close(pipe_fds[1]);
		new_pipe(pipe_fds);
		params->dup_fds[1][0] = pipe_fds[1];
		params->dup_fds[1][1] = STDOUT_FILENO;
		params->dup_fds_count = 2;
	}
}

void	pipe_while(int cmds_count, char *cmds_names[], char *envp[])
{
	t_fork_cmd_params	params;
	t_fork_infos		*cmds_pids;
	int					pipe_fds[2];
	int					i;

	cmds_pids = malloc(sizeof(t_fork_infos) * cmds_count + 1);
	params = init_params(envp);
	new_pipe(pipe_fds);
	i = 0;
	while (i < cmds_count)
	{
		reset_params(&params, cmds_names, pipe_fds, i);
		conditional_params(&params, pipe_fds, i, cmds_count);
		cmds_pids[i] = fork_exec_cmd(params);
		free_tab(params.cmd_args);
		i++;
	}
	close(pipe_fds[1]);
	while (--i)
		if (cmds_pids[i].pid)
			waitpid(cmds_pids->pid + i, cmds_pids->stat_loc, WUNTRACED);
	(free(cmds_pids), free(params.dup_fds[0]),
		free(params.dup_fds[1]), free(params.dup_fds));
}
