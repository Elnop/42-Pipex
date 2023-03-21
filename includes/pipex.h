/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lperroti <lperroti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 21:34:42 by lperroti          #+#    #+#             */
/*   Updated: 2023/03/20 21:46:11 by lperroti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "../libs/liblp_c/liblp.h"
# include "fcntl.h"
# include <stdio.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/wait.h>

typedef struct s_fork_cmd_params {
	char	*cmd_name;
	char	**cmd_args;
	char	**envp;
	int		**dup_fds;
	size_t	dup_fds_count;
	int		close_fd;
}	t_fork_cmd_params;

int		fork_exec_cmd(t_fork_cmd_params params);
void	free_tab(char **tab);
void	new_pipe(int pipe_fds[2]);
void	pipe_while(int cmds_count, char *cmds_names[], char *envp[]);
char	**get_paths(char *envp[]);

#endif