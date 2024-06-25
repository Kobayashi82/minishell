/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assign.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 19:18:48 by vzurera-          #+#    #+#             */
/*   Updated: 2024/05/01 16:42:46 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//	Free all assignments args and move the rest to the first position
static void	free_prev_nodes(t_args **node, t_args *current)
{
	t_args	*next_node;

	while (*node != current)
	{
		next_node = (*node)->next;
		free((*node)->arg);
		free(*node);
		*node = next_node;
	}
}

//	Delete an arg from an args list
static void	delete_arg(t_args **arg, t_token *token)
{
	t_args	*tmp;

	free((*arg)->arg);
	if ((*arg)->next)
	{
		tmp = (*arg)->next;
		(*arg)->arg = (*arg)->next->arg;
		(*arg)->next = (*arg)->next->next;
		if ((*arg)->next)
			(*arg)->next->prev = *arg;
		free(tmp);
	}
	else
	{
		if ((*arg)->prev)
			(*arg)->prev->next = NULL;
		else
			token->args_lst = NULL;
		free(*arg);
	}
}

//	Create a new shell variable
static void	assign_shell_vars(t_data *data, t_token *token)
{
	t_args	*current;

	current = token->args_lst;
	while (token->args_lst && current && current->arg)
	{
		if (!validate_var(data, safe_strdup(data, current->arg), false))
			delete_arg(&current, token);
		else
			break ;
	}
}

//	Check and modify an args list if not all args are variable assignments
static bool	is_vars_set(t_args **current, t_args **prev)
{
	bool	all_vars;

	all_vars = true;
	while (*current)
	{
		if (!is_var((*current)->arg))
		{
			all_vars = false;
			break ;
		}
		*prev = *current;
		*current = (*current)->next;
	}
	return (all_vars);
}

//	Check if there are variable assignments in the token
int	assign_vars(t_data *data, t_token *token)
{
	t_args	*current;
	t_args	*prev;
	t_args	*node;
	bool	all_vars;

	current = token->args_lst;
	prev = NULL;
	all_vars = is_vars_set(&current, &prev);
	if (all_vars)
	{
		assign_shell_vars(data, token);
		return (1);
	}
	else if (prev)
	{
		node = token->args_lst;
		free_prev_nodes(&node, current);
		token->args_lst = current;
	}
	return (0);
}
