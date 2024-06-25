/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 23:35:20 by vzurera-          #+#    #+#             */
/*   Updated: 2024/05/07 22:49:55 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//	Add a variable to the array
static void	add_export_var(t_data *data, char ***envp, char *name)
{
	char	**n_envp;
	int		n;

	n = 0;
	while ((*envp)[n] != NULL)
		n++;
	n_envp = ft_realloc(*envp, (n + 1) * sizeof(char *), \
		(n + 2) * sizeof(char *));
	if (!n_envp)
		exit_error(data, NO_MEMORY, 1001, NULL);
	n_envp[n] = safe_strdup(data, name);
	n_envp[n + 1] = NULL;
	*envp = n_envp;
}

//	Add export shell variables for printing
static void	create_export(t_data *data, char ***envp)
{
	t_var	*current;

	current = data->vars;
	while (current)
	{
		if (current->ex)
			add_export_var(data, envp, current->name);
		current = current->next;
	}
}

//	clone and sort before printing
void	export_sort(t_data *data, char **envp, int is_p)
{
	char	**s_envp;
	char	*tmp;
	char	**i;
	char	**j;

	s_envp = environment_clone(data, envp);
	create_export(data, &s_envp);
	i = s_envp;
	while (*(i + 1))
	{
		j = i + 1;
		while (*j)
		{
			if (ft_strcmp(*i, *j) > 0)
			{
				tmp = *i;
				*i = *j;
				*j = tmp;
			}
			j++;
		}
		i++;
	}
	export_print(data, s_envp, is_p);
	free_array(s_envp);
}
