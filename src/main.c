/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmora-ro <jmora-ro@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 12:05:19 by jmora-ro          #+#    #+#             */
/*   Updated: 2025/11/23 15:17:19 by jmora-ro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/pipex.h"

static void	init_child1(int infile, int *p_fd, char **envp, char *cmd)
{
	dup2(infile, STDIN_FILENO); // Instead of taking info from your keyboard, take it from the infile
	dup2(p_fd[1], STDOUT_FILENO); // Instead of writing to my screen, write to p_fd[1] the write end of my pipe
	close(p_fd[0]);
	close(p_fd[1]);
	close(infile);
	execute_command(cmd, envp);
}

static void init_child2(int outfile, int *p_fd, char **envp, char *cmd)
{
	dup2(p_fd[0], STDIN_FILENO); // Instead of taking information from my keyboard, take it from the read end of my pipe
	dup2(outfile, STDOUT_FILENO); // Instead of writing information to the screen, write to outfile
	close(p_fd[0]);
	close(p_fd[1]);
	close(outfile);
	execute_command(cmd, envp);
}

static void	init_files_and_pipe(int *infile, int *outfile, int *p_fd, char **argv)
{
	*infile = open(argv[1], O_RDONLY);
	if (*infile == -1)
		handle_exit(2);
	*outfile = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (*outfile == -1)
		handle_exit(2);
	if (pipe(p_fd) == -1)
		handle_exit(2);
}

static void close_all_fds(int *p_fd, int infile, int outfile)
{
	close(p_fd[0]);
	close(p_fd[1]);
	close(infile);
	close(outfile);
}

int	main(int argc, char **argv, char **envp)
{
	int		p_fd[2];
	int		infile;
	int		outfile;
	pid_t	pid1;
	pid_t	pid2;

	if (argc != 5)
		handle_exit(1);
	init_files_and_pipe(&infile, &outfile, p_fd, argv);
	pid1 = fork();
	if (pid1 == -1)
		handle_exit(2);
	if (pid1 == 0)
		init_child1(infile, p_fd, envp, argv[2]);
	pid2 = fork();
	if (pid2 == -1)
		handle_exit(2);
	if (pid2 == 0)
		init_child2(outfile, p_fd, envp, argv[3]);
	close_all_fds(p_fd, infile, outfile);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
	return (0);
}
