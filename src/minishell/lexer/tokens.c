/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 20:20:31 by vzurera-          #+#    #+#             */
/*   Updated: 2024/05/07 13:07:24 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//	Clears a list of tokens
void	tokens_clear(t_data *data)
{
	t_token	*curr;
	t_token	*next;

	curr = data->tokens;
	while (curr)
	{
		if (!curr->next && (prev_cmd(curr) && prev_cmd(curr)->fd[1] != -1))
			close(prev_cmd(curr)->fd[1]);
		curr = curr->next;
	}
	curr = data->tokens;
	while (curr)
	{
		next = curr->next;
		free(curr->cmd);
		free_array(curr->args);
		args_list_clear(&curr->args_lst);
		redir_list_clear(&curr->redir);
		redir_list_clear(&curr->s_redir);
		free(curr);
		curr = next;
	}
	data->tokens = NULL;
}

//	Add a new token
void	tokens_add(t_data *data, t_token *new_token)
{
	t_token	*current;

	if (!new_token)
		return ;
	if (!data->tokens)
		data->tokens = new_token;
	else
	{
		current = data->tokens;
		while (current->next)
			current = current->next;
		current->next = new_token;
		new_token->prev = current;
	}
}

//	Create a new token with the defined properties
t_token	*tokens_new(t_data *data, char *value, int type)
{
	t_token	*new_token;

	if ((!value || ft_isspace_s(value)) && type == 0)
	{
		if (value)
			free(value);
		return (NULL);
	}
	new_token = ft_calloc(1, sizeof(t_token));
	if (!new_token)
	{
		if (value)
			free (value);
		exit_error(data, NO_MEMORY, 1000, NULL);
	}
	new_token->data = data;
	new_token->cmd = value;
	new_token->type = type;
	new_token->fd[0] = -1;
	new_token->fd[1] = -1;
	return (new_token);
}

//	Add a command token
int	tokens_add_cmd(t_data *data, char **input)
{
	t_token	*token;
	char	*line;
	int		i;

	line = *input;
	i = 0;
	while (line && line[i] && !(ft_memchr("<>()|&;", line[i], 7)
			&& !is_esc(line, i)))
	{
		if (line[i] == '$' && line[i + 1] == '(' && skip_paren(line, &i))
			return (1);
		else if ((line[i] == '\'' || line[i] == '"') && skip_quotes(line, &i))
			return (1);
		else
			i++;
	}
	token = tokens_new(data, ft_substr(line, 0, i), CMD);
	tokens_add(data, token);
	*input += i;
	return (0);
}

//	Process an input and create a list of tokens
int	tokens_set(t_data *data)
{
	char	*input;
	int		i;

	input = data->input;
	while (input && *input)
	{
		i = 0;
		if (ft_isspace(*input))
			input++;
		if (ft_memchr("<>()|&;", *input, 7)
			&& !is_esc(data->input, input - data->input))
		{
			if (tokens_add_separator(data, &input))
				return (1);
		}
		else if (tokens_add_cmd(data, &input))
			return (1);
		input += i;
	}
	return (0);
}
