/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lperroti <lperroti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 21:34:42 by lperroti          #+#    #+#             */
/*   Updated: 2023/03/29 23:28:33 by lperroti         ###   ########.fr       */
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
	int		close_fd;
	int		fd_in;
	int		fd_out;
}	t_fork_cmd_params;

typedef struct s_fork_infos {
	pid_t	pid;
	int		*stat_loc;
}	t_fork_infos;

t_fork_infos	fork_exec_cmd(t_fork_cmd_params params);
void			free_tab(char **tab);
void			new_pipe(int pipe_fds[2]);
int				pipe_while(size_t cmds_count, char *cmds_names[], char *envp[]);
char			**get_paths(char *envp[]);
bool			check_cmds(int cmds_count, char *cmds_names[], char *envp[]);
int				here_doc(char *limiter);

#endif