/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lperroti <lperroti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 22:27:58 by lperroti          #+#    #+#             */
/*   Updated: 2023/03/21 20:02:39 by lperroti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

bool	exec_cmd(char *cmd_name, char **cmd_args, char *envp[])
{
	char	**paths;
	size_t	i;

	if (!access(cmd_name, F_OK))
	{
		execve(cmd_name, cmd_args, envp);
		return (true);
	}
	paths = get_paths(envp);
	if (!paths)
		return (false);
	i = 0;
	while (paths[i])
	{
		if (!lp_strcat(paths + i, cmd_name))
			break ;
		if (!access(paths[i], F_OK))
		{
			execve(paths[i], cmd_args, envp);
			return (free_tab(paths), true);
		}
		i++;
	}
	return (free_tab(paths), false);
}

void	dup_fds(t_fork_cmd_params params)
{
	size_t	i;

	i = 0;
	while (i < params.dup_fds_count)
	{
		dup2(params.dup_fds[i][0], params.dup_fds[i][1]);
		i++;
	}
}

void	close_fds(t_fork_cmd_params params)
{
	if (params.close_fd != -1)
		close(params.close_fd);
}

int	fork_exec_cmd(t_fork_cmd_params params)
{
	int	cpid;

	cpid = fork();
	if (cpid == -1)
		return (-1);
	else if (!cpid)
	{
		dup_fds(params);
		close_fds(params);
		if (!exec_cmd(params.cmd_name, params.cmd_args, params.envp))
			return (-1);
		while (params.dup_fds_count--)
			close(params.dup_fds[params.dup_fds_count][0]);
		exit(0);
	}
	else
	{
		while (params.dup_fds_count--)
			close(params.dup_fds[params.dup_fds_count][0]);
		return (cpid);
	}
}
