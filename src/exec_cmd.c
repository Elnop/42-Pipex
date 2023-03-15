/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lperroti <lperroti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 22:27:58 by lperroti          #+#    #+#             */
/*   Updated: 2023/03/15 03:46:55 by lperroti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

char	**get_paths(char *envp[])
{
	char	**paths;
	size_t	i;

	while (*envp)
	{
		if (lp_strncmp(*envp, "PATH=", 5))
		{
			envp++;
			continue ;
		}
		paths = lp_split(*envp + 5, ':');
		i = 0;
		while (paths[i])
			lp_strcat(paths + i++, "/");
		return (paths);
	}
	return (NULL);
}

bool	exec_cmd(char *cmd_name, char **cmd_args, char *envp[])
{
	char	**paths;

	paths = get_paths(envp);
	while (*paths)
	{
		if (!lp_strcat(paths, cmd_name))
			return (false);
		if (!access(*paths, F_OK))
		{
			execve(*paths, cmd_args, envp);
			break ;
		}
		paths++;
	}
	return (true);
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

void	fork_exec_cmd(t_fork_cmd_params params)
{
	int	cpid;
	// int	status;

	cpid = fork();
	if (cpid == -1)
	{
		perror("fork error");
		exit(EXIT_FAILURE);
	}
	else if (!cpid)
	{
		dup_fds(params);
		close_fds(params);
		exec_cmd(params.cmd_name, params.cmd_args, params.envp);
		exit(0);
	}
	// else {
	// 	waitpid(cpid, &status, 0);
	// }
}