/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lperroti <lperroti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 21:33:19 by lperroti          #+#    #+#             */
/*   Updated: 2023/03/29 23:45:27 by lperroti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	check_fds(int fd_in, int fd_out)
{
	if (fd_in == -1)
	{
		lp_putendl_fd("can't open file", 2);
		if (fd_out != -1)
			close(fd_out);
		exit(1);
	}
	if (fd_out == -1)
	{
		lp_putendl_fd("can't open file", 2);
		if (fd_in != -1)
			close(fd_in);
		exit(1);
	}
}

void	change_std_fds(int *argc, char **argv[])
{
	int		fd_in;
	int		fd_out;

	if (!lp_strncmp((*argv)[0], "here_doc", 9) && (*argc)-- && (*argv)++)
	{
		if (*argc < 4
			&& lp_putendl_fd("exemple: ./pipex here_doc S cmd1 cmd2 file2\n", 2))
			exit(0);
		fd_in = here_doc(**argv);
		fd_out = open((*argv)[*argc - 1],
				O_WRONLY | O_CREAT | O_APPEND, S_IRWXU);
	}
	else
	{
		fd_in = open((*argv)[0], O_RDONLY);
		fd_out = open((*argv)[*argc - 1],
				O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
	}
	check_fds(fd_in, fd_out);
	(dup2(fd_in, STDIN_FILENO), dup2(fd_out, STDOUT_FILENO));
	(close(fd_in), close(fd_out));
}

int	main(int argc, char *argv[], char *envp[])
{
	if (argc-- && argv++ && argc < 4)
	{
		lp_putendl_fd("Use exemple: ./pipex file1 cmd1 cmd2 file2\n", 2);
		return (0);
	}
	change_std_fds(&argc, &argv);
	if (check_cmds(argc - 2, argv + 1, envp))
		return (pipe_while(argc - 2, argv + 1, envp));
	lp_putendl_fd("command not found", 2);
	exit(127);
}
