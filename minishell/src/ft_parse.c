/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcaumont <bcaumont@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 14:13:50 by garside           #+#    #+#             */
/*   Updated: 2025/06/01 21:53:47 by bcaumont         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../octolib/includes/libft.h"

void	free_env_list(t_env *new_list)
{
	t_env	*temp;

	while (new_list)
	{
		temp = new_list;
		new_list = new_list->next;
		free(temp->name);
		if (temp->content)
			free(temp->content);
		free(temp);
	}
}

t_token	*get_next_token(t_data *data, int *i)
{
	if (data->input[*i] == '>' || data->input[*i] == '<')
		return (handle_redirection(data->input, i));
	if (data->input[*i] == '|')
		return (handle_pipe(i));
	return (handle_cmd_or_arg(data, i));
}

int	valid_parse(t_data *data)
{
	t_token	*tmp;

	tmp = data->token;
	while (tmp)
	{
		if (tmp->type != WORD && !tmp->next && tmp->type != PIPE)
		{
			g_status = 2;
			return (printf("%s `newline`\n", ERR_SYNT), 1);
		}
		if (tmp->type == PIPE && tmp->next && tmp->next->type == PIPE)
		{
			g_status = 2;
			return (printf("%s `|`\n", ERR_SYNT), 1);
		}
		if ((tmp->type != WORD && tmp->type != PIPE) && (tmp->next
				&& tmp->next->type != WORD))
		{
			g_status = 2;
			return (printf("%s `%s`\n", ERR_SYNT, tmp->next->value), 1);
		}
		tmp = tmp->next;
	}
	return (0);
}

static void	print_pipe_error(void)
{
	g_status = 2;
	printf("%s `|'\n", ERR_SYNT);
}

int	parse(t_data *data)
{
	t_token	*token;

	if (!data->input)
		return (1);
	data->token = ft_lexer(data);
	if (!data->token)
		return (1);
	token = data->token;
	if (valid_parse(data) == 1)
		return (1);
	if (token->type == PIPE)
		return (printf("%s `|'\n", ERR_SYNT), 1);
	while (token && token->next)
		token = token->next;
	if (token->type == PIPE)
		return (print_pipe_error(), 1);
	data->cmd_list = parse_tokens(data);
	if (!data->cmd_list)
		return (1);
	if (!data->cmd_list->args && !data->cmd_list->outfile
		&& !data->cmd_list->infile)
		return (1);
	return (0);
}
