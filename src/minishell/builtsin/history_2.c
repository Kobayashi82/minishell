/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 16:18:28 by vzurera-          #+#    #+#             */
/*   Updated: 2024/04/30 16:19:03 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Restore history
void	restore_history(HIST_ENTRY **old_history)
{
	int	i;

	i = 0;
	while (old_history && old_history[i])
		add_history(old_history[i++]->line);
	i = 0;
	while (old_history[i])
	{
		free(old_history[i]->line);
		free(old_history[i++]);
	}
	free(old_history);
}

// Copy history
HIST_ENTRY	**copy_history(void)
{
	HIST_ENTRY	**old_history;
	HIST_ENTRY	**new_history;
	int			i;
	int			j;

	i = 0;
	j = 0;
	old_history = history_list();
	while (old_history[i])
		i++;
	new_history = malloc((i + 1) * sizeof(HIST_ENTRY *));
	while (new_history && j < i)
	{
		new_history[j] = malloc(sizeof(HIST_ENTRY));
		new_history[j]->line = ft_strdup(old_history[j]->line);
		new_history[j++]->data = NULL;
	}
	new_history[i] = NULL;
	return (new_history);
}
