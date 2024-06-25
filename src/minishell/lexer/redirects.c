/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 12:42:20 by vzurera-          #+#    #+#             */
/*   Updated: 2024/05/07 18:41:55 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//	Get the next token command
t_token	*next_cmd_token(t_token *token)
{
	t_token	*current;

	current = token;
	while (current && (current->type > 0 && current->type < 6))
		current = current->next;
	if (current && current->type == CMD)
		return (current);
	if (token->prev && token->prev->type == CPAR)
	{
		token->redirection = true;
		return (token);
	}
	token->type = CMD;
	token->redirection = true;
	return (token);
}

//	Delete redirections tokens
static void	delete_token(t_token *token)
{
	t_token	*current;
	t_token	*tmp;

	if (!token)
		return ;
	current = token;
	while (current && current->type < 6)
	{
		tmp = current->next;
		free(current->cmd);
		args_list_clear(&current->args_lst);
		free(current);
		current = tmp;
	}
}

//	Set the redirections list to the parenthesis
static int	redirects_par(t_token *token, t_token *cmd, t_token *next)
{
	token->prev->redir = token->redir;
	token->redir = NULL;
	cmd = token->prev;
	delete_token(token);
	cmd->next = next;
	return (0);
}

//	If there are redirections only, set the first redirection as CMD
static void	redirects_set(t_token *token, t_token *cmd)
{
	token->type = CMD;
	free(token->cmd);
	args_list_clear(&token->args_lst);
	token->cmd = cmd->cmd;
	token->args_lst = cmd->args_lst;
	cmd->cmd = NULL;
	cmd->args_lst = NULL;
}

//	Process redirections for the current next command token
int	redirects(t_token *token, bool redirect)
{
	t_token	*cmd;
	t_token	*next;

	if (!token || token->type > 5)
		return (0);
	if (redirect)
		token->redir = get_redir(token);
	cmd = next_cmd_token(token);
	next = next_separator(token, 5);
	if (token->redirection && token->prev && token->prev->type == CPAR)
		return (redirects_par(token, cmd, next));
	if (token->type != CMD)
		redirects_set (token, cmd);
	delete_token(token->next);
	token->next = next;
	if (token->next)
		token->next->prev = token;
	return (0);
}
