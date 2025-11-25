/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmora-ro <jmora-ro@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 12:05:19 by jmora-ro          #+#    #+#             */
/*   Updated: 2025/11/25 12:28:08 by jmora-ro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/pipex.h"

static void	init_child1(int *p_fd, char **argv, char **envp)
{
	int	infile;

	infile = open(argv[1], O_RDONLY, 0777);
	if (infile == -1)
	{
		perror(argv[1]);
		exit(1);
	}
	dup2(infile, STDIN_FILENO);
	dup2(p_fd[1], STDOUT_FILENO);
	close(p_fd[0]);
	close(p_fd[1]);
	close(infile);
	execute_command(argv[2], envp);
}

static void	init_child2(int *p_fd, char **argv, char **envp)
{
	int	outfile;

	outfile = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (outfile == -1)
	{
		perror(argv[4]);
		exit(1);
	}
	dup2(p_fd[0], STDIN_FILENO);
	dup2(outfile, STDOUT_FILENO);
	close(p_fd[0]);
	close(p_fd[1]);
	close(outfile);
	execute_command(argv[3], envp);
}

static void	init_pipe(int *p_fd)
{
	if (pipe(p_fd) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
}

static int	cleanup_and_wait(int *p_fd, pid_t pid1, pid_t pid2)
{
	int	status;

	status = 0;
	close(p_fd[0]);
	close(p_fd[1]);
	waitpid(pid1, &status, 0);
	waitpid(pid2, &status, 0);
	return (status >> 8);
}

int	main(int argc, char **argv, char **envp)
{
	int		p_fd[2];
	pid_t	pid1;
	pid_t	pid2;

	if (argc != 5)
	{
		ft_putstr_fd("Wrong number of arguments", 2);
		exit(1);
	}
	init_pipe(p_fd);
	pid1 = fork();
	if (pid1 == -1)
		handle_exit();
	if (pid1 == 0)
		init_child1(p_fd, argv, envp);
	pid2 = fork();
	if (pid2 == -1)
		handle_exit();
	if (pid2 == 0)
		init_child2(p_fd, argv, envp);
	return (cleanup_and_wait(p_fd, pid1, pid2));
}
