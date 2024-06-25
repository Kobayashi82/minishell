/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 17:45:42 by vzurera-          #+#    #+#             */
/*   Updated: 2024/04/30 16:54:16 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//	Return true if the current token is part of a pipe redirection
bool	is_pipe(t_token *token)
{
	t_token	*next;
	t_token	*prev;

	next = next_separator(token, 7);
	prev = token->prev;
	return ((next && next->type == PIPE) || (prev && prev->type == PIPE));
}

//	Close a FD and set it to -1
void	close_fd(int *fd)
{
	close(*fd);
	*fd = -1;
}

//	Close pipes (if exists)
int	close_fds(t_token *token, int mode)
{
	if (!token)
		return (0);
	if (!mode)
	{
		if (token->fd[0] != -1)
			close_fd(&token->fd[0]);
		if (token->fd[1] != -1)
			close_fd(&token->fd[1]);
		if (!token->data->main && !token->data->is_subshell
			&& next_separator(token, 7)
			&& next_separator(token, 7)->fd[0] != -1)
			close_fd(&next_separator(token, 7)->fd[0]);
	}
	else
	{
		if (next_separator(token, 7) && next_separator(token, 7)->fd[0] != -1)
			close_fd(&next_separator(token, 7)->fd[0]);
	}
	return (1);
}

//	Expand variables, assign variables and expand wildcards
int	expand(t_token *token, bool is_last_arg)
{
	t_token	*current;
	bool	is_var;

	is_var = false;
	current = token;
	while (current && current->type < 6)
	{
		if (!is_last_arg)
			is_var = assign_vars(current->data, current);
		reconstruct_args(current, current->args_lst);
		expand_wildcards(current->data, current->args_lst);
		current = current->next;
	}
	if (is_var && !token->data->main && !token->data->is_subshell)
	{
		close_fds(token, 0);
		exit_error(token->data, NOTHING, 1000, NULL);
	}
	if (!token->args_lst)
	{
		token->data->last_status = 0;
		return (1);
	}
	return (0);
}

//	Execute the current token
int	execute(t_token *token)
{
	if (!token || g_signal != 0)
		return (1);
	token->data->last_token = token;
	if (token->type < 8 && !check_no_exec(token) && !is_parenthesis(token))
	{
		if (!is_pipe(token) && token->args_lst && check_builtin(token))
		{
			if (expand(token, false))
				return (0);
			redirects(token, true);
			build_path(token->data, token);
			if (!set_redirections(token, false) && !token->redirection)
				builtsin(token);
			reset_redirections(token->data, true, true, false);
			return (0);
		}
		else if (forking(token))
			return (!redirects(token, false));
		redirects(token, false);
		expand(token, true);
		set_last_arg(token);
	}
	return (0);
}
