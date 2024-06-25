/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_executer.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 13:58:45 by vzurera-          #+#    #+#             */
/*   Updated: 2024/05/06 10:24:09 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//	Get the next token separator
t_token	*next_separator(t_token *token, int type)
{
	t_token	*current;
	int		pars;

	if (!token)
		return (NULL);
	current = token;
	while (current)
	{
		if (current->type == OPAR)
		{
			current = current->next;
			pars = 1;
			while (current && pars)
			{
				pars += (current->type == OPAR);
				pars -= (current->type == CPAR);
				current = current->next;
			}
		}
		if (current && current->type > type)
			return (current);
		if (current)
			current = current->next;
	}
	return (NULL);
}

//	Get the next token that needs to be executed
static t_token	*next_cmd(t_token *token)
{
	t_token	*next;

	while (token && ((token->type == AND && token->data->last_status)
			|| (token->type == OR && !token->data->last_status)))
		token = next_separator(token->next, 8);
	if (!token || token->type > 7)
		return (token);
	next = next_separator(token, 7);
	if (next && (next->type == AND || next->type == OR))
		token->wait = true;
	if (next && next->type == PIPE && pipe(next->fd) == -1)
	{
		exit_error(token->data, PIPE_FAIL, 1, NULL);
		return (NULL);
	}
	else if (next && next->type == PIPE)
		token->fd[1] = next->fd[1];
	if (token->prev && token->prev->type == PIPE)
		token->fd[0] = token->prev->fd[0];
	return (token);
}

static void	get_status(t_data *data, int status, int *pid, bool check)
{
	*pid = 0;
	if (!check || (!data->exit || data->exit_custom_code == 1000))
	{
		if (WIFEXITED(status))
			data->last_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			data->last_status = 128 + WTERMSIG(status);
	}
}

//	Wait for the child processes to end
static void	wait_tokens(t_data *data, int status)
{
	t_token	*current;

	data->reset_status = 0;
	current = data->tokens;
	while (current)
	{
		if ((current->type == CMD || current->type == OPAR) && current->pid)
		{
			waitpid(current->pid, &status, 0);
			get_status(data, status, &current->pid, true);
		}
		current = current->next;
	}
	if (data->main)
		signal(SIGQUIT, SIG_IGN);
}

//	Start the execution process
void	pre_execute(t_data *data, int status)
{
	heredocs(data);
	data->reset_status = 0;
	data->last_token = data->tokens;
	while (!execute(next_cmd(data->last_token))
		&& data->last_token && !data->exit && g_signal == 0)
	{
		if (data->last_token->type == SMCOL || data->last_token->type == SMAND)
			wait_tokens(data, 0);
		if ((data->last_token->type == CMD || data->last_token->type == OPAR)
			&& data->last_token->wait && data->last_token->pid)
		{
			waitpid(data->last_token->pid, &status, 0);
			get_status(data, status, &data->last_token->pid, false);
			if (data->main)
				signal(SIGQUIT, SIG_IGN);
			if (g_signal == 130)
				break ;
		}
		data->last_token = data->last_token->next;
		if (data->last_token && data->last_token->prev->type == OPAR)
			data->last_token = next_separator(data->last_token->prev, OPAR);
	}
	wait_tokens(data, 0);
}
