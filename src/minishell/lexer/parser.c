/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 15:16:33 by vzurera-          #+#    #+#             */
/*   Updated: 2024/04/29 01:57:15 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//	Add the arg to the args list
static int	arg_add(t_data *data, t_token *current, char *arg)
{
	t_args	*new_arg;
	t_args	*last_arg;

	new_arg = ft_calloc(1, sizeof(t_args));
	if (!new_arg)
	{
		free(arg);
		exit_error(data, NO_MEMORY, 1000, NULL);
	}
	new_arg->arg = arg;
	new_arg->token = current;
	new_arg->data = data;
	new_arg->next = NULL;
	new_arg->prev = NULL;
	last_arg = current->args_lst;
	if (!last_arg)
		current->args_lst = new_arg;
	else
	{
		while (last_arg->next)
			last_arg = last_arg->next;
		new_arg->prev = last_arg;
		last_arg->next = new_arg;
	}
	return (0);
}

//	Split a command (or redirection) into args and store them in a list
static int	parser_cmd(t_data *data, t_token *current, char **cmd, int *i)
{
	char	*str;

	while (*cmd && (*cmd)[*i])
	{
		if (ft_isspace((*cmd)[*i]) && !is_esc(*cmd, *i))
		{
			str = ft_substr(*cmd, 0, *i);
			if (!str)
				exit_error(data, NO_MEMORY, 1000, NULL);
			if (!ft_isspace_s(str))
				arg_add(data, current, str);
			else
				free (str);
			*cmd += *i + 1;
			*i = 0;
		}
		else if ((*cmd)[*i] == '(' && skip_paren(*cmd, i))
			return (1);
		else if (((*cmd)[*i] == '\'' || (*cmd)[*i] == '"')
			&& skip_quotes(*cmd, i))
			return (1);
		else
			(*i)++;
	}
	return (0);
}

//	Process all the tokens and get theirs args
int	parser(t_data *data)
{
	t_token	*current;
	char	*cmd;
	int		i;

	current = data->tokens;
	while (current)
	{
		i = 0;
		cmd = current->cmd;
		parser_cmd(data, current, &cmd, &i);
		if (i > 0)
			arg_add(data, current, ft_substr(cmd, 0, i));
		current = current->next;
	}
	return (0);
}
