/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 23:49:47 by vzurera-          #+#    #+#             */
/*   Updated: 2024/05/07 22:50:26 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//	Check and set the content of the variable
static int	set_content(t_data *data, char *value)
{
	char	*new_content;
	int		len;
	int		i;

	i = -1;
	len = 0;
	while (value[len])
	{
		if (value[len] == '"' || value[len] == '$' || value[len] == '\\')
			len++;
		len++;
	}
	new_content = malloc((len * 3) + 1);
	len = 0;
	if (!new_content)
		return (1);
	while (value[++i])
	{
		if (value[i] == '"' || value[i] == '$' || value[i] == '\\')
			new_content[len++] = '\\';
		new_content[len++] = value[i];
	}
	new_content[len] = '\0';
	print(data, 1, ft_strjoin("=\"", new_content, 2), FJ);
	return (0);
}

//	Check and set the name of the variable
static void	set_name(t_data *data, char *name, char	*value)
{
	int		len;

	len = 0;
	len = name - value;
	name = safe_strdup(data, value);
	name[len] = '\0';
	print(data, 1, ft_strjoin("declare -x ", name, 0), FJ);
	len = set_content(data, &name[len + 1]);
	free(name);
	if (len)
		exit_error(data, NO_MEMORY, 1, NULL);
	print(data, 1, "\"\n", J);
}

//	Print exported environments variables
void	export_print(t_data *data, char **envp, int is_p)
{
	char	*name;
	int		i;

	i = -1;
	print(data, 1, NULL, R);
	while (envp && envp[++i])
	{
		if (!is_p && !ft_strncmp(envp[i], "_=", 2))
			continue ;
		name = ft_memchr(envp[i], '=', ft_strlen(envp[i]));
		if (name)
			set_name(data, name, envp[i]);
		else
		{
			print(data, 1, ft_strjoin("declare -x ", envp[i], 0), FJ);
			print(data, 1, "\n", J);
		}
	}
	print(data, 1, NULL, P);
}
