/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 19:16:19 by vzurera-          #+#    #+#             */
/*   Updated: 2024/05/11 01:26:08 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//	Check the syntax for the variable name
static int	variable_syntax(t_data *data, char *var, char *name, int is_name)
{
	char	*content;

	if (name && is_name == 1)
	{
		free(var);
		free(name);
		return (0);
	}
	if (name && is_name > 1)
	{
		content = safe_strdup(data, "[EXPORT_ONLY]");
		set_var(data, name, content, false);
		free(var);
		free(name);
		free(content);
		return (0);
	}
	if (!name)
	{
		print(data, 2, ft_strjoin("minishell: export: `", var, 0), FR);
		print(data, 2, "': not a valid identifier\n", P);
		free(var);
	}
	return (1);
}

//	Check for forbidden chars in a variable name
bool	forbidden_chars(char *new_name, bool unset)
{
	if (!ft_isalpha(*new_name) && *new_name != '_')
		return (true);
	while (new_name && *new_name)
	{
		if (*new_name == '+' && !*(new_name + 1) && !unset)
			break ;
		if (!ft_isalnum(*new_name) && *new_name != '\\' && *new_name != '_')
			return (true);
		if (*new_name == '\\'
			&& (!ft_isdigit(*(new_name + 1)) && *(new_name + 1) != '_'))
			return (true);
		new_name++;
	}
	return (false);
}

//	Validate a variable syntax
static char	*validate_str(t_data *data, char *str, bool is_name)
{
	char	*new_str;

	if (!str)
		return (NULL);
	else if (is_name && forbidden_chars(str, false))
		return (free(str), NULL);
	else if (!is_name)
		tilde(data, &str);
	new_str = ft_strdup(str);
	free(str);
	return (new_str);
}

//	Create or modify an existing variable (shell or environment)
int	validate_var(t_data *data, char *var, bool env)
{
	char	*name;
	char	*content;
	char	*pos;

	name = NULL;
	content = NULL;
	trim_esc(data->last_token, var, false, false);
	pos = ft_memchr(var, '=', ft_strlen(var));
	if (pos && (pos - var) > 0)
	{
		name = validate_str(data, ft_substr(var, 0, (pos - var)), true);
		content = validate_str(data, ft_substr(var, (pos - var) + 1, \
			ft_strlen(pos + 1)), false);
	}
	if (env)
	{
		if (!pos && !name)
			name = validate_str(data, ft_strdup(var), true);
		if (!name || !*name || (!pos && (!shell_get(data->vars, name, true)
					|| shell_get(data->vars, name, true)->ex)))
			return (free(content), variable_syntax(data, var, name, \
				(!pos && !shell_get(data->vars, name, 1)) + (env * 2)));
	}
	set_var(data, name, content, env);
	return (free(var), free(name), free(content), 0);
}
