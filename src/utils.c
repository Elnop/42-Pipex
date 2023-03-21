/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lperroti <lperroti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 22:26:38 by lperroti          #+#    #+#             */
/*   Updated: 2023/03/20 21:58:23 by lperroti         ###   ########.fr       */
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

void	new_pipe(int pipe_fds[2])
{
	if (pipe(pipe_fds) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
}
