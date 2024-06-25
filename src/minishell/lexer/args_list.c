/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_list.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 17:50:39 by vzurera-          #+#    #+#             */
/*   Updated: 2024/04/29 01:57:15 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//	Clears all the args in an args list
void	args_list_clear(t_args **args)
{
	t_args	*current;
	t_args	*next;

	current = *args;
	while (current)
	{
		next = current->next;
		if (current->arg)
			free(current->arg);
		free(current);
		current = next;
	}
	*args = NULL;
}

//	Merge two args lists (used for vars expansion and wildcards)
void	args_list_merge(t_args *args, t_args **new_args)
{
	t_args	*last;

	if (new_args && *new_args)
	{
		last = *new_args;
		while (last->next)
			last = last->next;
		if (args->next)
			last->next = args->next;
		if (args->arg)
			free(args->arg);
		args->arg = (*new_args)->arg;
		args->next = (*new_args)->next;
		free(*new_args);
		*new_args = NULL;
	}
}

//	Add the arg to the args list
static void	arg_add(t_token *token, t_args **args_lst, char *arg)
{
	t_args	*new_arg;
	t_args	*last_arg;

	new_arg = ft_calloc(1, sizeof(t_args));
	if (!new_arg)
	{
		free(arg);
		exit_error(token->data, NO_MEMORY, 1000, NULL);
	}
	new_arg->arg = arg;
	new_arg->token = token;
	new_arg->data = token->data;
	new_arg->next = NULL;
	new_arg->prev = NULL;
	last_arg = *args_lst;
	if (!last_arg)
		*args_lst = new_arg;
	else
	{
		while (last_arg->next)
			last_arg = last_arg->next;
		new_arg->prev = last_arg;
		last_arg->next = new_arg;
	}
}

//	Split a command (or redirection) into args and store them in a list
static int	parse_arg(t_token *token, t_args **args_lst, char *arg, int i)
{
	char	*str;

	while (arg && arg[i])
	{
		if (ft_isspace(arg[i]) && !is_esc(arg, i))
		{
			str = ft_substr(arg, 0, i);
			if (!str)
				exit_error(token->data, NO_MEMORY, 1000, NULL);
			if (!ft_isspace_s(str))
				arg_add(token, args_lst, str);
			else
				free (str);
			arg += i + 1;
			i = 0;
		}
		else if ((arg[i] == '(' && skip_paren(arg, &i))
			|| ((arg[i] == '\'' || arg[i] == '"') && skip_quotes(arg, &i)))
			return (1);
		else
			i++;
	}
	if (i > 0)
		arg_add(token, args_lst, ft_substr(arg, 0, i));
	return (0);
}

int	reconstruct_args(t_token *token, t_args *args)
{
	t_args	*args_lst;

	args_lst = NULL;
	if (args == NULL)
		return (0);
	if (reconstruct_args(token, args->next))
		return (1);
	if (parse_arg(token, &args_lst, args->arg, 0))
	{
		args_list_clear(&args_lst);
		return (1);
	}
	if (args_lst && args_lst->next)
		args_list_merge(args, &args_lst);
	else
		args_list_clear(&args_lst);
	return (0);
}
