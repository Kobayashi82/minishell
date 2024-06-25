/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 23:45:44 by vzurera-          #+#    #+#             */
/*   Updated: 2024/05/11 02:13:51 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//	Check the syntax for unclosed quotes
static int	quoted_syntax(t_data *data)
{
	char	*line;
	char	quote[2];
	int		i;

	i = 0;
	quote[0] = 0;
	quote[1] = '\0';
	line = data->input;
	while (line && line[i])
	{
		if ((line[i] == '"' || line[i] == '\'') && !is_esc(line, i))
		{
			if (quote[0] == 0)
				quote[0] = line[i];
			else if (quote[0] == line[i])
				quote[0] = 0;
		}
		i++;
	}
	if (quote[0] == 0)
		return (0);
	data->last_status = 2;
	print(data, 2, "minishell: unexpected EOF while looking for matching `", R);
	print(data, 2, ft_strjoin(quote, "'\n", 0), FP);
	return (1);
}

//	Process input
void	process_input(t_data *data)
{
	signal(SIGINT, sigint_killer);
	if (data->input && !ft_isspace_s(data->input) && ft_strcmp(data->input, ":")
		&& ft_strcmp(data->input, "!") && !check_last_cmd(data)
		&& !quoted_syntax(data))
	{
		if (history_length == 0 || (history_get(history_length)
				&& history_get(history_length)->line
				&& ft_strcmp(history_get(history_length)->line, data->input)))
			add_history(data->input);
		if (!tokenize(data))
			pre_execute(data, 0);
	}
	if (data->main && (g_signal == 129 || g_signal == 130))
	{
		if (g_signal == 129)
			g_signal = 130;
		else
			write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
	}
	data->hd_t = 0;
	if (data->main)
		clear_tmp(data);
	data->hd_n = -1;
}

//	Show the prompt and read the input
int	read_input(t_data *data)
{
	char	*input;

	first_start(data);
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	g_signal = open_quote(data, NULL, true) + open_pars(NULL, true);
	set_prompt(data);
	input = readline(data->prompt);
	free(data->prompt);
	if (!input)
		return (1);
	clean_comments(input);
	data->input = ft_strdup(input);
	data->extra = 1;
	tokenize(data);
	while (!data->extra && open_line(data, input))
		read_sub_input(data, &input);
	free(input);
	if (data->extra != -1 && g_signal == 0)
		process_input(data);
	free(data->input);
	data->input = NULL;
	data->hd_t = 0;
	return (0);
}

//	Check the type of error for the command passed as argument
void	argument_error(t_data *data, char *arg)
{
	char	*cmd;
	int		i;

	i = -1;
	while (arg[++i])
		if (arg[i] != '/' && arg[i] != '.')
			break ;
	if (arg[i])
		data->input = create_path(data, arg);
	else
		data->input = safe_strdup(data, arg);
	cmd = data->input;
	if (!cmd)
		exit_error(data, NOTHING, 1000, NULL);
	if (is_directory(cmd))
		exit_error(data, START_DIR, 1126, cmd);
	if (access(cmd, F_OK) == -1)
		exit_error(data, OPEN_NO_FILE, 1127, cmd);
	exit_error(data, START_BIN, 1126, cmd);
}
