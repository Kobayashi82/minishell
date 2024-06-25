/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   separators.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 16:42:09 by vzurera-          #+#    #+#             */
/*   Updated: 2024/05/06 12:09:15 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//	Search for the next word in a string
static char	*next_word(char *input, int *i, bool full)
{
	int	start;

	while (input && ft_isspace(input[*i]))
		(*i)++;
	start = *i;
	while (input && input[*i] && (full || (!full && (!ft_isspace(input[*i])
					|| (ft_isspace(input[*i]) && is_esc(input, *i)))))
		&& !(ft_memchr("<>()|&;", input[*i], 7) && !is_esc(input, *i)))
	{
		if (input[*i] == '$' && input[*i + 1] == '(' && skip_paren(input, i))
			return (NULL);
		else if ((input[*i] == '\'' || input[*i] == '"')
			&& skip_quotes(input, i))
			return (NULL);
		else
			(*i)++;
	}
	return (ft_substr(input, start, *i - start));
}

//	Get the previous command on a token list
static t_token	*prev_cmd_token(t_data *data)
{
	t_token	*current;

	current = data->tokens;
	while (current && current->next)
		current = current->next;
	while (current && current->prev && (current->type > 0 && current->type < 6))
		current = current->prev;
	if (current && current->type == CMD)
		return (current);
	return (NULL);
}

//	Add a redirection token with the corresponding file name for input/output
static int	add_redirect(t_data *data, char **input, int type)
{
	t_token	*token;
	char	*cmd;
	int		i;

	i = 0;
	(*input) += 1 + (type == DLT || type == TLT || type == DGT) + (type == TLT);
	cmd = next_word(*input, &i, false);
	*input += i;
	token = tokens_new(data, cmd, type);
	if (!token)
		return (exit_error(data, SYNTAX, 1, NULL));
	tokens_add(data, token);
	token = prev_cmd_token(data);
	if (token)
	{
		i = 0;
		cmd = next_word(*input, &i, true);
		if (cmd)
		{
			*input += i;
			token->cmd = ft_strjoin_c(token->cmd, cmd, " ", 3);
		}
	}
	return (0);
}

//	Add a separator token
static int	add_separator(t_data *data, char **input, int type)
{
	t_token	*token;

	token = tokens_new(data, NULL, type);
	if (!token)
		return (exit_error(data, SYNTAX, 1, false));
	tokens_add(data, token);
	if (type == TLT)
		(*input)++;
	if (type == DLT || type == TLT || type == DGT || type == AND || type == OR)
		(*input)++;
	(*input)++;
	return (0);
}

//	Handles separators and redirectors
int	tokens_add_separator(t_data *data, char **input)
{
	if (!ft_strncmp(*input, "<<<", 3))
		return (add_redirect(data, input, TLT));
	else if (!ft_strncmp(*input, "<<", 2))
		return (add_redirect(data, input, DLT));
	else if (!ft_strncmp(*input, ">>", 2))
		return (add_redirect(data, input, DGT));
	else if (!ft_strncmp(*input, "<", 1))
		return (add_redirect(data, input, LT));
	else if (!ft_strncmp(*input, ">", 1))
		return (add_redirect(data, input, GT));
	else if (!ft_strncmp(*input, "(", 1))
		return (add_separator(data, input, OPAR));
	else if (!ft_strncmp(*input, ")", 1))
		return (add_separator(data, input, CPAR));
	else if (!ft_strncmp(*input, "&&", 2))
		return (add_separator(data, input, AND));
	else if (!ft_strncmp(*input, "||", 2))
		return (add_separator(data, input, OR));
	else if (!ft_strncmp(*input, ";", 1))
		return (add_separator(data, input, SMCOL));
	else if (!ft_strncmp(*input, "&", 1))
		return (add_separator(data, input, SMAND));
	else if (!ft_strncmp(*input, "|", 1))
		return (add_separator(data, input, PIPE));
	return (exit_error(data, SYNTAX, 0, NULL));
}
