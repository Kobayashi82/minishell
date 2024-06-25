/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 20:53:33 by vzurera-          #+#    #+#             */
/*   Updated: 2024/05/07 18:55:55 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//	Retrieves a shell variable
t_var	*shell_get(t_var *vars, char *name, bool all)
{
	char	*str;

	str = ft_strdup(name);
	if (all && str && *str && str[ft_strlen(str) - 1] == '+')
		str[ft_strlen(str) - 1] = '\0';
	while (str && vars && vars->name)
	{
		if (!ft_strcmp(vars->name, str) && (all || !vars->ex))
		{
			free(str);
			return (vars);
		}
		vars = vars->next;
	}
	free(str);
	return (NULL);
}

//	Sets a shell variable
int	shell_set(t_data *data, t_var **vars, char *name, char *content)
{
	t_var	*current;

	if (!name)
		return (1);
	current = join_shell_var(vars, name, &content);
	if (current)
	{
		free(current->content);
		current->content = ft_strdup(content);
	}
	else
	{
		current = safe_calloc(data, 1, sizeof(t_var));
		current->name = ft_strdup(name);
		current->content = ft_strdup(content);
		current->ex = !(ft_strcmp(current->content, "[EXPORT_ONLY]"));
		if (!current->name || !current->content)
			return (free(current->name), free(current), \
				exit_error(data, NO_MEMORY, 1000, NULL), 1);
		current->next = *vars;
		if (*vars)
			(*vars)->prev = current;
		*vars = current;
	}
	return (0);
}

//	Deletes a shell variable
void	shell_unset(t_var **vars, char *name)
{
	t_var	*var;

	var = shell_get(*vars, name, true);
	if (var)
	{
		if (var->next)
			var->next->prev = var->prev;
		if (var->prev)
			var->prev->next = var->next;
		else
			*vars = var->next;
		free(var->name);
		free(var->content);
		free(var);
	}
}

//	Clears shell variables
void	shell_clear(t_var **vars)
{
	t_var	*current;

	while (*vars)
	{
		current = (*vars)->next;
		free((*vars)->name);
		free((*vars)->content);
		free(*vars);
		*vars = current;
	}
	*vars = NULL;
}

//	Clones shell variables
t_var	*shell_clone(t_data *data, t_var *vars)
{
	t_var	*new_var;
	t_var	*current;
	t_var	*clone;

	new_var = NULL;
	current = NULL;
	while (vars)
	{
		clone = safe_calloc(data, 1, sizeof(t_var));
		clone->name = ft_strdup(vars->name);
		clone->content = ft_strdup(vars->content);
		if (!clone->name || !clone->content)
			return (free(clone->name), free(clone), \
				exit_error(data, NO_MEMORY, 1000, NULL), NULL);
		clone->prev = current;
		if (current)
			current->next = clone;
		else
			new_var = clone;
		current = clone;
		vars = vars->next;
	}
	return (new_var);
}
