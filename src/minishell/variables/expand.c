/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/30 21:08:58 by vzurera-          #+#    #+#             */
/*   Updated: 2024/05/05 21:02:16 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//	Return a variable if exists
static char	*expand_var(t_data *data, char *name)
{
	if (!name)
		ft_strdup("");
	if (!ft_strcmp(name, "RANDOM"))
		return (ft_itoa(rand() % 32768));
	if (!ft_strcmp(name, "SECONDS"))
		return (ft_itoa((int) difftime(time(NULL), data->started)));
	if (shell_get(data->vars, name, false))
		return (ft_strdup(shell_get(data->vars, name, false)->content));
	return (ft_strdup(env_get(data->envp, name)));
}

//	Get and replace a variable with his value
int	exval(t_data *data, char **arg, int *i, char *name)
{
	char	*cont;

	if (!name)
		exit_error(data, NO_MEMORY, 1000, NULL);
	if (!ft_strcmp(name, "$"))
		cont = ft_itoa(data->pid);
	else if (!ft_strcmp(name, "?"))
		cont = ft_itoa(data->last_status);
	else if (!ft_strcmp(name, "_"))
	{
		if (ft_strstr(env_get(data->envp, "_"), "/env"))
			cont = ft_strdup("env");
		else
			cont = ft_strdup(env_get(data->envp, "_"));
	}
	else if (!ft_strcmp(name, "0"))
		cont = ft_strdup("minishell");
	else
		cont = expand_var(data, name);
	if (!cont)
		*arg = ft_replace(*arg, i, ft_strlen(name) + 1, "");
	else
		*arg = ft_replace(*arg, i, ft_strlen(name) + 1, add_quote(data, &cont));
	return (free(cont), free(name), 0);
}

static int	expand_vars_utils(t_data *data, char **s, int *i)
{
	if ((*s)[*i] == '$' && !is_esc(*s, *i) && !data->extra
		&& ((*s)[*i + 1] == '"' || (*s)[*i + 1] == '\''))
	{
		ft_memcpy(*s + *i, *s + *i + 1, ft_strlen(*s + *i + 1) + 1);
		return (1);
	}
	else if ((*s)[*i] == '$' && !is_esc(*s, *i)
			&& ft_memchr("$?_0123456789*", (*s)[*i + 1], 14)
			&& ((*s)[*i + 1] != '_' || ((*s)[*i + 1] == '_'
			&& (!(*s)[*i + 2] || ft_isspace((*s)[*i + 2])))))
	{
		exval(data, s, i, ft_substr(&(*s)[*i + 1], 0, 1));
		return (1);
	}
	return (0);
}

//	Expand and replace all variables in a string
static void	expand_vars_arg(t_token *token, char **s, int i, int e)
{
	token->data->extra = 0;
	pre_subshell_var(token->data, s);
	if (!token->no_exec && token->data->last_status == 131)
		token->no_exec = true;
	while ((*s) && (*s)[i])
	{
		if ((*s)[i] == '"' && !is_esc(*s, i))
			token->data->extra = !token->data->extra;
		if (!token->data->extra && (*s)[i] == '\'' && skip_quotes(*s, &i))
			return ;
		if (expand_vars_utils(token->data, s, &i))
			continue ;
		else if ((*s)[i] == '$' && !is_esc(*s, i) && (*s)[i + 1]
			&& !ft_isspace((*s)[i + 1]) && (*s)[i + 1] != '\\'
			&& (*s)[i + 1] != '%' && (*s)[i + 1] != '"'
			&& (*s)[i + 1] != '=' && (*s)[i + 1] != ':')
		{
			e = i + 1;
			while ((*s)[e] && (ft_isalnum((*s)[e]) || (*s)[e] == '_'))
				e++;
			exval(token->data, s, &i, ft_substr(&(*s)[i + 1], 0, (e - i) - 1));
		}
		else
			i++;
	}
}

//	Expand and replace all variables in an args list
void	expand_vars(t_token *token)
{
	t_args	*current;
	t_args	*tmp;
	t_args	*prev;

	prev = NULL;
	current = token->args_lst;
	while (current)
	{
		if (current->arg && *current->arg)
			expand_vars_arg(token, &current->arg, 0, 0);
		if (!current->arg || !*current->arg)
		{
			if (prev)
				prev->next = current->next;
			else
				token->args_lst = current->next;
			tmp = current;
			current = current->next;
			free(tmp->arg);
			free(tmp);
			continue ;
		}
		prev = current;
		current = current->next;
	}
}
