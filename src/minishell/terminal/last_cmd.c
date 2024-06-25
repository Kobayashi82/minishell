/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   last_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 22:46:16 by vzurera-          #+#    #+#             */
/*   Updated: 2024/05/05 17:00:34 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//	Replace !! from input with the last input
static int	replace_input(t_data *data, char *last_cmd, bool *asked)
{
	char	*line;
	int		i;

	line = safe_strdup(data, data->input);
	i = 0;
	while (line && line[i])
	{
		if (line[i] == '\'' && skip_quotes(line, &i))
			return (free(line), 1);
		else if (line[i] == '!' && line[i + 1] == '!' && !is_esc(line, i))
		{
			*asked = true;
			if (!last_cmd)
			{
				exit_error(data, CMD_LAST, 0, NULL);
				return (free(line), 1);
			}
			line = ft_replace(line, &i, 2, last_cmd);
		}
		else
			i++;
	}
	free(data->input);
	data->input = line;
	return (0);
}

//	Check if must be replaced !! from input
int	check_last_cmd(t_data *data)
{
	HIST_ENTRY	*entry;
	char		*last_cmd;
	bool		asked;

	if (data->input && !ft_strcmp(data->input, "."))
	{
		print(data, 2, "minishell: .: filename argument required\n", RP);
		data->last_status = 2;
		return (1);
	}
	last_cmd = NULL;
	asked = false;
	if (history_length)
	{
		entry = history_get(history_length);
		if (entry && entry->line)
			last_cmd = entry->line;
	}
	if (replace_input(data, last_cmd, &asked))
		return (1);
	if (asked)
		print(data, 1, ft_strjoin(data->input, "\n", 0), FRP);
	return (0);
}

//	Set the variable '_' to the las executed argument
void	set_last_arg(t_token *token)
{
	t_args	*current;
	char	*tmp;

	if (!token)
		return ;
	current = token->args_lst;
	while (current && token->type == CMD && !token->redirection)
	{
		if (!current->next)
		{
			if (!ft_strcmp(current->arg, "env"))
			{
				tmp = create_path(token->data, "env");
				env_set(&token->data->envp, "_", tmp);
				free(tmp);
			}
			else
				env_set(&token->data->envp, "_", current->arg);
			return ;
		}
		current = current->next;
	}
	env_set(&token->data->envp, "_", "");
}
