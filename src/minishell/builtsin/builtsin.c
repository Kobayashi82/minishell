/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtsin.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 22:28:22 by vzurera-          #+#    #+#             */
/*   Updated: 2024/05/06 13:00:53 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//	Check if a token (or subtoken) is a builtin
bool	check_builtin(t_token *token)
{
	t_token	*current;

	current = token;
	while (current && (current->type > 0 && current->type < 6))
		current = current->next;
	if (current && current->type == CMD)
		return (is_builtin(current->args_lst->arg) || is_vars(current));
	return (true);
}

//	Return a value if the passed argument is a builtin
bool	is_builtin(char *arg0)
{
	if (arg0 && *arg0)
	{
		if (!ft_strcmp(arg0, "echo"))
			return (true);
		if (!ft_strcmp(arg0, "env"))
			return (true);
		if (!ft_strcmp(arg0, "unset"))
			return (true);
		if (!ft_strcmp(arg0, "export"))
			return (true);
		if (!ft_strcmp(arg0, "cd"))
			return (true);
		if (!ft_strcmp(arg0, "pwd"))
			return (true);
		if (!ft_strcmp(arg0, "exit"))
			return (true);
		if (!ft_strcmp(arg0, "about") || !ft_strcmp(arg0, "help"))
			return (true);
		if (!ft_strcmp(arg0, "banner"))
			return (true);
		if (!ft_strcmp(arg0, "history"))
			return (true);
	}
	return (false);
}

//	Execute more built-ins
static int	more_builtsin(t_token *token)
{
	if (!ft_strcmp(token->args[0], "pwd"))
		return (pwd(token, token->args));
	if (!ft_strcmp(token->args[0], "exit"))
		return (exit_minishell(token, token->args));
	if (!ft_strcmp(token->args[0], "about"))
		return (about(token->data));
	if (!ft_strcmp(token->args[0], "help"))
		return (help(token->data));
	if (!ft_strcmp(token->args[0], "history"))
		return (history(token, token->args));
	if (!ft_strcmp(token->args[0], "banner"))
	{
		print_banner(token->data);
		return (token->data->last_status);
	}
	return (-1);
}

//	Execute built-ins
int	builtsin(t_token *token)
{
	int	status;

	status = -1;
	set_last_arg(token);
	if (token->args && token->args[0] && token->args[0][0])
	{
		if (!ft_strcmp(token->args[0], "echo"))
			status = echo(token, token->args);
		else if (!ft_strcmp(token->args[0], "env"))
			status = env(token->data, token->args);
		else if (!ft_strcmp(token->args[0], "unset"))
			status = unset(token->data, token->args);
		else if (!ft_strcmp(token->args[0], "export"))
			status = export(token, token->args);
		else if (!ft_strcmp(token->args[0], "cd"))
			status = cd(token, token->args);
		else
			status = more_builtsin(token);
		if (status != -1)
			token->data->last_status = status;
	}
	if (status == -1)
		env_set(&token->data->envp, "_", "");
	return (status != -1);
}
