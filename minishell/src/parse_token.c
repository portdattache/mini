/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: broboeuf <broboeuf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 01:00:55 by broboeuf          #+#    #+#             */
/*   Updated: 2025/06/02 01:02:11 by broboeuf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	init_new_command_if_needed(t_cmd **head, t_cmd **curr)
{
	if (!*curr)
	{
		*curr = new_cmd_node();
		if (!*head)
			*head = *curr;
	}
}

static void	handle_pipe_token(t_cmd **curr)
{
	(*curr)->next = new_cmd_node();
	*curr = (*curr)->next;
}

static void	handle_token(t_cmd *curr, t_token *token, int *skip_next_word)
{
	if (token->type == WORD)
		add_arg(curr, token->value);
	else if ((token->type == REDIRECTION_IN || token->type == HEREDOC)
		&& token->next)
	{
		add_redir(&curr->infile, token->next->value, token->type);
		*skip_next_word = 1;
	}
	else if ((token->type == REDIRECTION_OUT || token->type == APPEND)
		&& token->next)
	{
		add_redir(&curr->outfile, token->next->value, token->type);
		*skip_next_word = 1;
	}
}

t_cmd	*parse_tokens(t_data *data)
{
	t_cmd	*head;
	t_cmd	*curr;
	t_token	*token;
	int		skip_next_word;

	head = NULL;
	curr = NULL;
	token = data->token;
	skip_next_word = 0;
	while (token)
	{
		init_new_command_if_needed(&head, &curr);
		if (skip_next_word)
		{
			skip_next_word = 0;
			token = token->next;
			continue ;
		}
		if (token->type == PIPE)
			handle_pipe_token(&curr);
		else
			handle_token(curr, token, &skip_next_word);
		token = token->next;
	}
	return (head);
}
