/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmora-ro <jmora-ro@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 11:55:38 by jmora-ro          #+#    #+#             */
/*   Updated: 2025/11/24 15:09:49 by jmora-ro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "../lib/libft/libft.h"
# include <fcntl.h>
# include <stdio.h>
# include <sys/_types/_pid_t.h>

void	handle_exit(void);
char	**parse_command(char *cmd_string);
char	*find_command_path(char *cmd, char **envp);
char	*get_path_from_envp(char **envp);
void	free_split(char **arr);
void	execute_command(char *cmd, char **envp);
#endif
