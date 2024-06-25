/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 13:18:23 by vzurera-          #+#    #+#             */
/*   Updated: 2024/04/29 01:57:15 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//	Clears all the redirections in a redirection list
void	redir_list_clear(t_redir **redir)
{
	t_redir	*current;
	t_redir	*next;

	current = *redir;
	while (current)
	{
		next = current->next;
		if (current->file)
			free(current->file);
		free(current);
		current = next;
	}
	*redir = NULL;
}

//	Add a redirection node to a list of redirections
static void	add_redir(t_redir *new, t_redir **redir)
{
	t_redir	*last;

	last = *redir;
	if (!last)
		*redir = new;
	else
	{
		while (last->next)
			last = last->next;
		new->prev = last;
		last->next = new;
	}
}

//	Create a new redirection node
static void	new_redir(t_token *token, t_redir **redir)
{
	t_redir	*new;
	char	*file;

	new = safe_calloc(token->data, 1, sizeof(t_redir));
	new->type = token->type;
	file = token->cmd;
	if (!token->heredoc)
		file = token->args_lst->arg;
	new->ambiguous = false;
	new->expand = token->expand;
	if (!token->heredoc && (!token->args_lst || !*token->args_lst->arg
			|| token->args_lst->next))
	{
		file = token->cmd;
		new->ambiguous = true;
	}
	if (!file)
		free(new);
	else
	{
		new->file = ft_strdup(file);
		add_redir(new, redir);
	}
}

//	Create a list or redirections
t_redir	*get_redir(t_token *token)
{
	t_redir	*redir;
	t_token	*current;
	t_token	*next;

	redir = NULL;
	current = token;
	while (current)
	{
		next = current->next;
		if (current->type > 5)
			break ;
		if (current->type != CMD)
			new_redir(current, &redir);
		current = next;
	}
	return (redir);
}
