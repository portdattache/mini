/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_here_doc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: broboeuf <broboeuf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 19:49:33 by garside           #+#    #+#             */
/*   Updated: 2025/05/27 17:09:46 by broboeuf         ###   ########.fr       */
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

void	fill_here_doc_file(char *delimitor, char *file)
{
	char	*str;
	int		fd;

	fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return ;
	signal(SIGINT, handle_sigint);
	g_status = 0;
	while (1)
	{
		str = readline("> ");
		if (g_status == 130)
		{
			free(str);
			close(fd);
			unlink(file);
			return ;
		}
		if (str == NULL)
		{
			ft_printf("bash: warning: here-document delimited by end-of-file (wanted `%s')\n",
				delimitor);
			break ;
		}
		if (ft_strcmp(str, delimitor) == 0)
		{
			free(str);
			break ;
		}
		ft_putstr_fd(str, fd);
		ft_putchar_fd('\n', fd);
		free(str);
	}
	close(fd);
	signal(SIGINT, handle_sigint);
}

char	*get_here_doc(char *str)
{
	char *file_name;
	char *delimitor;

	delimitor = ft_strdup(str);
	if (!delimitor)
		return (NULL);
	made_new_file(&file_name);
	fill_here_doc_file(delimitor, file_name);
	free(delimitor);
	if (g_status == 130)
	{
		unlink(file_name);
		free(file_name);
		return (NULL);
	}
	return (file_name);
}