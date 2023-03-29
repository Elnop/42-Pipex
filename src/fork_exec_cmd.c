/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_exec_cmd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lperroti <lperroti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 22:27:58 by lperroti          #+#    #+#             */
/*   Updated: 2023/03/28 21:15:40 by lperroti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

bool	exec_cmd(char *cmd_name, char **cmd_args, char *envp[])
{
	char	**paths;
	size_t	i;

	if (!access(cmd_name, F_OK))
		execve(cmd_name, cmd_args, envp);
	paths = get_paths(envp);
	if (!paths)
		return (false);
	i = 0;
	while (paths[i])
	{
		if (!lp_strcat(paths + i, cmd_name))
			break ;
		if (!access(paths[i], F_OK))
			execve(paths[i], cmd_args, envp);
		i++;
	}
	return (free_tab(paths), false);
}

void	dup_fds(t_fork_cmd_params params)
{
	if (params.fd_in != -1)
	{
		dup2(params.fd_in, STDIN_FILENO);
		close(params.fd_in);
	}
	if (params.fd_out != -1)
	{
		dup2(params.fd_out, STDOUT_FILENO);
		close(params.fd_out);
	}
}

void	close_fds(t_fork_cmd_params params)
{
	if (params.close_fd != -1)
		close(params.close_fd);
}

t_fork_infos	fork_exec_cmd(t_fork_cmd_params params)
{
	t_fork_infos	fork_infos;
	int				stat_loc;

	fork_infos.stat_loc = &stat_loc;
	fork_infos.pid = fork();
	if (!fork_infos.pid)
	{
		dup_fds(params);
		close_fds(params);
		exec_cmd(params.cmd_name, params.cmd_args, params.envp);
		exit(0);
	}
	else
		return (fork_infos);
}
