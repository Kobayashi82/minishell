/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell_par.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 17:17:25 by vzurera-          #+#    #+#             */
/*   Updated: 2024/04/30 13:47:32 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//	Check if a command should not execute
bool	check_no_exec(t_token *token)
{
	t_token	*current;

	current = token;
	while (current && (current->type > 0 && current->type < 6))
		current = current->next;
	if (current && current->type == CMD && current->no_exec)
		return (true);
	return (false);
}

//	Find the previous command or redireccion for the last token;
t_token	*prev_cmd(t_token *token)
{
	t_token	*current;

	current = token;
	while (current)
	{
		if (current->type == CMD || (current->prev && current->prev->type > 5)
			|| !current->prev)
			return (current);
		current = current->prev;
	}
	return (NULL);
}

//	Call the subshell execution and replace the subshell command with output
static void	execute_subshell(t_data *data, t_token *token)
{
	t_data	sub_data;

	if (token->data->main)
		signal(SIGQUIT, sigquit_handler);
	token->prev->pid = fork();
	if (token->prev->pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGINT, sigint_killer);
		subshell_par_child(data, &sub_data, token);
	}
	close_fds(data->last_token, 0);
	if (token->prev->pid < 0)
		exit_error(data, FORK_FAIL, 1001, NULL);
}

//	Check if the command is in parenthesis and execute the subshell
bool	is_parenthesis(t_token *token)
{
	t_token	*current;

	if (token->type == OPAR)
	{
		execute_subshell(token->data, token->next);
		return (true);
	}
	if (token->args_lst && token->args_lst->arg)
	{
		current = token;
		while (current && current->type < 6)
		{
			if (current->args_lst && current->args_lst->arg)
				expand_vars(current);
			current = current->next;
		}
		return (false);
	}
	return (true);
}
