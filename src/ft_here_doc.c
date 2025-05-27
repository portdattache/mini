/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_here_doc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: broboeuf <broboeuf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 19:49:33 by garside           #+#    #+#             */
/*   Updated: 2025/05/27 19:55:18 by broboeuf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	made_new_file(char **name)
{
	static int	nb_file = 0;
	char		*nb_str;

	nb_str = ft_itoa(nb_file);
	*name = ft_strjoin("/tmp/here_doc_", nb_str);
	free(nb_str);
	nb_file++;
}

void	fill_here_doc_file(delimitor, file_name)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL); // Laisse Ctrl+C tuer ce processus
		fill_here_doc_file(delimitor, file_name);
		free(delimitor);
		exit(0); // enfant terminé
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		free(delimitor);
		if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		{
			unlink(file_name);
			free(file_name);
			g_status = 130;
			return (NULL);
		}
	}
	else
	{
		perror("fork");
		free(delimitor);
		return (NULL);
	}
}

char	*get_here_doc(char *str)
{
	char	*file_name;
	char	*delimitor;
	pid_t	pid;
	int		status;

	delimitor = ft_strdup(str);
	if (!delimitor)
		return (NULL);
	made_new_file(&file_name);
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		free(delimitor);
		return (NULL);
	}
	else if (pid == 0)
	{
		// Fils : lecture du heredoc
		signal(SIGINT, SIG_DFL);
		fill_here_doc_file(delimitor, file_name);
		free(delimitor);
		exit(0);
	}
	else
	{
		// Père : attente
		waitpid(pid, &status, 0);
		free(delimitor);
		if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		{
			unlink(file_name);
			free(file_name);
			g_status = 130;
			return (NULL);
		}
	}
	return (file_name);
}
