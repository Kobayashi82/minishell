/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 17:20:40 by vzurera-          #+#    #+#             */
/*   Updated: 2024/05/08 16:35:21 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//	Expand and replace all variables in a string
void	expand_heredoc(t_data *data, char **text)
{
	int		i;
	int		e;

	i = 0;
	pre_subshell_var(data, text);
	data->extra = 2;
	while ((*text) && (*text)[i])
	{
		if ((*text)[i] == '$' && !is_esc(*text, i) && (*text)[i + 1] == '(')
			i++;
		else if ((*text)[i] == '$' && !is_esc(*text, i)
			&& ft_memchr("$?_0", (*text)[i + 1], 4))
			exval(data, text, &i, ft_substr(&(*text)[i + 1], 0, 1));
		else if ((*text)[i] == '$' && !is_esc(*text, i))
		{
			e = i + 1;
			while ((*text)[e] && (ft_isalnum((*text)[e]) || (*text)[e] == '_'))
				e++;
			exval(data, text, &i, ft_substr(&(*text)[i + 1], 0, (e - i) - 1));
		}
		else
			i++;
	}
	data->extra = 0;
}

//	Add a line to the text of a heredoc
static char	*add_line(t_data *data, char *text, char *str_end, char *sep)
{
	char	*line;

	line = NULL;
	text = safe_strdup(data, "");
	while (g_signal != 130
		&& (!line || ft_strncmp(line, str_end, ft_strlen(line) - 1)
			|| ft_strlen(str_end) != ft_strlen(line) - 1))
	{
		text = ft_strjoin(text, line, 3);
		line = readline("> ");
		if (!line)
		{
			if (g_signal != 130)
				print(data, 1, sep, RP);
			print(data, 2, "minishell: warning: here-document ", R);
			print(data, 2, "delimited by end-of-file (wanted `", J);
			print(data, 2, ft_strjoin(str_end, "')\n", 0), FP);
			return (data->hd_t = 0, text);
		}
		line = ft_strjoin(line, "\n", 1);
	}
	free(line);
	if (g_signal != 130)
		print(data, 1, sep, RP);
	return (text);
}

//	Expand and replace all variables in a string
static void	delimiter(t_token *token, char *arg, char *cmd, int i)
{
	int	j;

	free(token->cmd);
	token->cmd = safe_strdup(token->data, arg);
	cmd = token->cmd;
	while (cmd && cmd[i])
	{
		if ((cmd[i] == '"' || cmd[i] == '\'') && !is_esc(cmd, i))
		{
			j = i;
			if (skip_quotes(cmd, &i))
				continue ;
			ft_memcpy(cmd + j, cmd + j + 1, ft_strlen(cmd + j + 1) + 1);
			ft_memcpy(cmd + i - 1, cmd + i, ft_strlen(cmd + i) + 1);
			i--;
		}
		else if (cmd[i] == '$' && !is_esc(cmd, i)
			&& (cmd[i + 1] == '"' || cmd[i + 1] == '\''))
			ft_memcpy(cmd + i, cmd + i + 1, ft_strlen(cmd + i + 1) + 1);
		else if (cmd[i] == '$' && !is_esc(cmd, i))
			i += 2;
		else
			i++;
	}
}

//	Create a heredoc
static int	heredoc(int fd, t_token *token, char *sep)
{
	char	*text;
	char	*file;

	if (!token->data->hd_t && g_signal != 130)
		print(token->data, 1, sep, RP);
	token->data->hd_t = 1;
	delimiter(token, token->args_lst->arg, NULL, 0);
	if (g_signal != 130)
	{
		print(token->data, 1, ft_strjoin("  Delimiter: ", token->cmd, 0), FR);
		print(token->data, 1, ft_strjoin("\n", sep, 0), FP);
	}
	text = add_line(token->data, NULL, token->cmd, sep);
	token->type = LT;
	token->expand = !token->quoted;
	token->heredoc = true;
	file = generate_tmp_name(token->data);
	free(token->cmd);
	token->cmd = file;
	fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd != -1 && text)
		write(fd, text, ft_strlen(text));
	else if (fd == -1)
		exit_error(token->data, OPEN_FAIL, 1, NULL);
	return (free(text), close(fd), 0);
}

//	Process all heredocs
void	heredocs(t_data *data)
{
	t_token		*current;
	HIST_ENTRY	**old_history;

	if (!data->main)
		return ;
	current = data->tokens;
	while (current && g_signal == 0)
	{
		if (current->type == DLT)
		{
			signal(SIGINT, sigint_handler);
			old_history = copy_history();
			clear_history();
			trim_arg(current, current->cmd);
			heredoc(0, current, \
				"  ——————————————————\n");
			restore_history(old_history);
			if (g_signal == 130)
				g_signal = 129;
			signal(SIGINT, sigint_killer);
		}
		current = current->next;
	}
}
