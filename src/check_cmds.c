/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_cmds.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lperroti <lperroti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 23:03:22 by lperroti          #+#    #+#             */
/*   Updated: 2023/03/29 23:44:00 by lperroti         ###   ########.fr       */
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
