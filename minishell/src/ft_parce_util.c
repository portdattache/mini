/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parce_util.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: broboeuf <broboeuf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 08:47:16 by garside           #+#    #+#             */
/*   Updated: 2025/06/02 01:00:42 by broboeuf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_redir_list(t_redir *redir)
{
	t_redir	*tmp;

	while (redir)
	{
		tmp = redir->next;
		if (redir->type == HEREDOC && redir->file)
			unlink(redir->file);
		if (redir->file)
			free(redir->file);
		free(redir);
		redir = tmp;
	}
}

void	free_cmd_list(t_data *data)
{
	t_cmd	*current;
	t_cmd	*next;

	if (!data || !data->cmd_list)
		return ;
	current = data->cmd_list;
	while (current)
	{
		next = current->next;
		if (current->outfile)
			free_redir_list(current->outfile);
		if (current->infile)
			free_redir_list(current->infile);
		if (current->args)
			free_split(current->args);
		free(current);
		current = next;
	}
	data->cmd_list = NULL;
}

t_cmd	*new_cmd_node(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->path = NULL;
	cmd->infile = NULL;
	cmd->outfile = NULL;
	cmd->next = NULL;
	cmd->pipe_fd[0] = -1;
	cmd->pipe_fd[1] = -1;
	cmd->saved_stdin = -1;
	cmd->saved_stdout = -1;
	return (cmd);
}

void	add_arg(t_cmd *cmd, char *value)
{
	int		i;
	char	**new_args;

	i = 0;
	if (cmd->args)
	{
		while (cmd->args[i])
			i++;
	}
	new_args = malloc(sizeof(char *) * (i + 2));
	if (!new_args)
		return ;
	i = 0;
	if (cmd->args)
	{
		while (cmd->args[i])
		{
			new_args[i] = ft_strdup(cmd->args[i]);
			i++;
		}
		free_split(cmd->args);
	}
	new_args[i++] = ft_strdup(value);
	new_args[i] = NULL;
	cmd->args = new_args;
}

void	add_redir(t_redir **redir_list, char *filename, int type)
{
	t_redir	*new_node;
	t_redir	*tmp;

	new_node = malloc(sizeof(t_redir));
	if (!new_node)
		return ;
	if (type == HEREDOC)
		new_node->file = get_here_doc(filename);
	else
		new_node->file = ft_strdup(filename);
	new_node->type = type;
	new_node->next = NULL;
	if (*redir_list == NULL)
		*redir_list = new_node;
	else
	{
		tmp = *redir_list;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new_node;
	}
}
