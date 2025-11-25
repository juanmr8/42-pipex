/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmora-ro <jmora-ro@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 12:40:28 by jmora-ro          #+#    #+#             */
/*   Updated: 2025/11/25 12:30:59 by jmora-ro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/pipex.h"

static char	*check_path_dir(char *dir, char *cmd)
{
	char	*temp;
	char	*full_path;

	temp = ft_strjoin(dir, "/");
	full_path = ft_strjoin(temp, cmd);
	free(temp);
	if (access(full_path, F_OK) == 0)
		return (full_path);
	free(full_path);
	return (NULL);
}

char	*find_command_path(char *cmd, char **envp)
{
	char	*path_env;
	char	**paths;
	char	*result;
	int		i;

	path_env = get_path_from_envp(envp);
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		result = check_path_dir(paths[i], cmd);
		if (result)
		{
			free_split(paths);
			return (result);
		}
		i++;
	}
	free_split(paths);
	return (NULL);
}

static void	validate_command(char *cmd)
{
	if (!cmd || !*cmd)
	{
		ft_putstr_fd("pipex: command not found: \n", 2);
		exit(127);
	}
}

static char	*get_command_path(char **args, char **envp)
{
	char	*path;

	path = find_command_path(args[0], envp);
	if (!path)
	{
		free_split(args);
		perror("command not found");
		exit(127);
	}
	return (path);
}

void	execute_command(char *cmd, char **envp)
{
	char	**args;
	char	*path;

	validate_command(cmd);
	args = parse_command(cmd);
	if (!args)
	{
		perror("parse_command failed");
		exit(EXIT_FAILURE);
	}
	path = get_command_path(args, envp);
	execve(path, args, envp);
	perror("execve failed");
	free_split(args);
	free(path);
	exit(EXIT_FAILURE);
}
