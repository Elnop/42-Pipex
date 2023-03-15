/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lperroti <lperroti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 21:34:42 by lperroti          #+#    #+#             */
/*   Updated: 2023/03/15 03:46:50 by lperroti         ###   ########.fr       */
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

void	fork_exec_cmd(t_fork_cmd_params params);

#endif