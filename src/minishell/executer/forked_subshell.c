/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forked_subshell.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 19:11:28 by vzurera-          #+#    #+#             */
/*   Updated: 2024/04/30 00:10:15 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//	Set the last token of the subshell, its redirections and pipes
static void	set_last_token(t_data *data, t_token *token, t_token *current)
{
	t_token	*last;

	last = NULL;
	if (current)
	{
		redirects(current->next, true);
		token->s_redir = current->redir;
		current->redir = NULL;
		last = current;
		if (current->prev)
			current->prev->next = NULL;
	}
	if (token->prev)
		token->prev->next = last;
	else
		data->tokens = last;
}

//	Find the las token to execute in the subshell
static t_token	*find_last_token(t_data *data, t_token *token)
{
	t_token	*current;
	int		pars;

	pars = 1;
	current = token;
	while (current && pars)
	{
		if (current->type == OPAR)
			pars++;
		else if (current->type == CPAR)
			pars--;
		if (!pars)
			break ;
		current = current->next;
	}
	if (!pars)
		set_last_token(data, token, current);
	token->prev = NULL;
	return (token);
}

//	Update ther data structure of all tokens
static void	update_data(t_data *sub_data)
{
	t_token	*current;
	t_args	*arg;

	current = sub_data->tokens;
	while (current)
	{
		current->data = sub_data;
		arg = current->args_lst;
		while (arg)
		{
			arg->data = sub_data;
			arg = arg->next;
		}
		if (current->type == OPAR)
			current = next_separator(current, OPAR);
		else
			current = current->next;
	}
}

//	Set the redirections for the subshell
static void	subshell_redirections(t_data *sub_data, t_token *tokens)
{
	t_redir	*redir;

	sub_data->tokens = tokens;
	sub_data->tokens->prev = NULL;
	redir = sub_data->tokens->redir;
	sub_data->tokens->redir = sub_data->tokens->s_redir;
	set_redirections(sub_data->tokens, true);
	sub_data->tokens->redir = redir;
	sub_data->tokens->fd[0] = -1;
	sub_data->tokens->fd[1] = -1;
	close(sub_data->bk_in);
	close(sub_data->bk_out);
	sub_data->bk_in = dup(STDIN_FILENO);
	sub_data->bk_out = dup(STDOUT_FILENO);
}

//	Process the tokens in the subshell command
void	subshell_par_child(t_data *data, t_data *sub_data, t_token *token)
{
	t_token	*tokens;
	int		in;
	int		out;

	data->main = false;
	tokens = find_last_token(data, token);
	tokens->fd[0] = data->last_token->fd[0];
	tokens->fd[1] = data->last_token->fd[1];
	data->last_token->fd[0] = -1;
	data->last_token->fd[1] = -1;
	in = dup(data->bk_in);
	out = dup(data->bk_out);
	initialize_subshell(data, sub_data, data->bk_out, false);
	tokens->data = sub_data;
	close(sub_data->bk_in);
	close(sub_data->bk_out);
	sub_data->bk_in = dup(in);
	sub_data->bk_out = dup(out);
	close(in);
	close(out);
	subshell_redirections(sub_data, tokens);
	update_data(sub_data);
	pre_execute(sub_data, 0);
	data_free(sub_data);
	exit(sub_data->last_status % 256);
}
