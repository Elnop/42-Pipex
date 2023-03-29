/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lperroti <lperroti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 23:25:45 by lperroti          #+#    #+#             */
/*   Updated: 2023/03/29 23:42:17 by lperroti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

int	here_doc(char *limiter)
{
	char	*line;
	int		pipe_fds[2];
	int		cpid;

	new_pipe(pipe_fds);
	cpid = fork();
	if (!cpid)
	{
		close(pipe_fds[0]);
		line = get_next_line(0);
		while (line && lp_strlen(line))
		{
			if (lp_strlen(line) - 1
				&& !lp_strncmp(line, limiter, lp_strlen(line) - 1))
				break ;
			lp_putstr_fd(line, pipe_fds[1]);
			free(line);
			line = get_next_line(0);
		}
		close(pipe_fds[1]);
		(free(line), exit(0));
	}
	close(pipe_fds[1]);
	wait(0);
	return (pipe_fds[0]);
}
