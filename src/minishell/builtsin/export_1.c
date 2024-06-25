/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 23:35:20 by vzurera-          #+#    #+#             */
/*   Updated: 2024/05/07 23:11:09 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//	Print help or version message
static int	export_message(t_data *d, int type)
{
	if (type)
	{
		print(d, 1, "export version 1.0 (part of minishell)\n\n", R);
		print(d, 1, "This is free software: ", J);
		print(d, 1, "you are free to change and redistribute it.\n", J);
		print(d, 1, "There is NO WARRANTY, to the extent permitted by law.", J);
		print(d, 1, "\n\nWritten by vzurera- (Kobayashi82)\n", P);
	}
	else
	{
		print(d, 1, "Usage: export -n [NAME=VALUE]... or export -p\n\n", R);
		print(d, 1, "[no option]		display a list of all exported "
			"variables\n-n			", J);
		print(d, 1, "remove the export property from each NAME\n", J);
		print(d, 1, "-p			display a list of all exported variables\n", J);
		print(d, 1, "--version		output version information\n", J);
		print(d, 1, "--help			display this help\n\n", P);
	}
	return (0);
}

//	Transfer or create a shell variable from environment
static void	transfer_env_var(t_data *data, char *arg)
{
	char	*content;
	char	*pos;

	if (shell_get(data->vars, arg, true)
		&& shell_get(data->vars, arg, true)->ex)
		shell_unset(&data->vars, arg);
	if (env_get(data->envp, arg))
	{
		content = ft_strdup(env_get(data->envp, arg));
		environment_unset(&data->envp, arg);
		shell_set(data, &data->vars, arg, content);
		free (content);
	}
	else
	{
		pos = ft_memchr(arg, '=', ft_strlen(arg));
		if (pos && (pos - arg) > 0)
		{
			pos = ft_substr(arg, 0, (pos - arg));
			environment_unset(&data->envp, pos);
			free(pos);
		}
		validate_var(data, safe_strdup(data, arg), false);
	}
}

//	Set or Unset a variable to environment
static int	export_set_unset(t_token *token, int status, int i, bool is_n)
{
	t_args	*curr;

	curr = token->args_lst;
	while (curr && curr->arg)
	{
		if (!is_n && i <= 0)
		{
			if (validate_var(token->data, \
				safe_strdup(token->data, curr->arg), true))
				status = 1;
		}
		else if (i <= 0)
			transfer_env_var(token->data, curr->arg);
		i--;
		curr = curr->next;
	}
	return (status);
}

//	Gets the options of the command
static bool	export_options(char *arg, bool *is_n, bool *is_p)
{
	int		i;
	bool	tmp_n;
	bool	tmp_p;

	i = 0;
	tmp_p = *is_p;
	tmp_n = *is_n;
	if (arg && arg[i] == '-')
	{
		while (arg[++i])
		{
			if (arg[i] == 'n')
				tmp_n = true;
			else if (arg[i] == 'p')
				tmp_p = true;
			else
				return (false);
		}
		*is_n = tmp_n;
		*is_p = tmp_p;
		return (true);
	}
	return (false);
}

//	Execute export built in
int	export(t_token *token, char **args)
{
	bool	is_n;
	bool	is_p;
	int		i;

	is_n = false;
	is_p = false;
	i = 0;
	if (!ft_strcmp(args[1], "--help"))
		return (export_message(token->data, 0));
	else if (!ft_strcmp(args[1], "--version"))
		return (export_message(token->data, 1));
	while (args && args[++i])
	{
		if (export_options(args[i], &is_n, &is_p))
			continue ;
		if (!is_p)
			return (export_set_unset(token, 0, i, is_n));
	}
	export_sort(token->data, token->data->envp, is_p);
	return (0);
}
