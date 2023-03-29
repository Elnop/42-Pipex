/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_while.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lperroti <lperroti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/18 02:48:43 by lperroti          #+#    #+#             */
/*   Updated: 2023/03/28 21:15:44 by lperroti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

int	wait_childs(t_fork_infos *cmds_pids, size_t cmd_count)
{
	int		status;
	size_t	i;

	i = 0;
	while (i < cmd_count)
	{
		waitpid(cmds_pids[i].pid, &status, 0);
		i++;
	}
	return (WIFEXITED(status) & WEXITSTATUS(status));
}

void	set_params(t_fork_cmd_params *params, int pipe_fds[2], size_t i,
	size_t cmds_count)
{
	if (!i)
	{
		new_pipe(pipe_fds);
		params->fd_in = -1;
		params->fd_out = pipe_fds[1];
		params->close_fd = pipe_fds[0];
	}
	else if (i == cmds_count - 1)
	{
		params->fd_in = pipe_fds[0];
		params->fd_out = -1;
		params->close_fd = -1;
		close(pipe_fds[1]);
	}
	else
	{
		close(pipe_fds[1]);
		params->fd_in = pipe_fds[0];
		new_pipe(pipe_fds);
		params->fd_out = pipe_fds[1];
		params->close_fd = pipe_fds[0];
	}
}

int	pipe_while(size_t cmds_count, char *cmds[], char *envp[])
{
	t_fork_cmd_params	params;
	t_fork_infos		*cmds_pids;
	int					pipe_fds[2];
	size_t				i;
	int					status;

	cmds_pids = malloc(sizeof(t_fork_infos) * cmds_count);
	params.envp = envp;
	i = 0;
	while (i < cmds_count)
	{
		params.cmd_args = lp_split(cmds[i], ' ');
		params.cmd_name = *params.cmd_args;
		set_params(&params, pipe_fds, i, cmds_count);
		cmds_pids[i] = fork_exec_cmd(params);
		if (params.fd_in > -1)
			close(params.fd_in);
		free_tab(params.cmd_args);
		i++;
	}
	status = wait_childs(cmds_pids, cmds_count);
	free(cmds_pids);
	return (status);
}
