/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 19:59:09 by vzurera-          #+#    #+#             */
/*   Updated: 2024/05/11 01:33:51 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//	Print main process error messages
static void	catastrophic_msg(t_data *data, int error, char *val)
{
	if (error == NO_MEMORY)
		print(data, 2, "CATASTROPHIC - No memory left on the device\n", P);
	if (error == FORK_FAIL)
		print(data, 2, "CATASTROPHIC - fork failed\n", P);
	if (error == START_ARGS)
		print(data, 2, "-c: option requires an argument\n", P);
	if (error == START_BIN)
	{
		print(data, 2, ft_strjoin(val, ": ", 0), FJ);
		print(data, 2, "cannot execute binary file\n", P);
	}
	if (error == START_DIR)
	{
		print(data, 2, ft_strjoin(val, ": ", 0), FJ);
		print(data, 2, "is a directory\n", P);
	}
	if (error == SHLVL_HIGH)
	{
		print(data, 2, ft_strjoin("warning: shell level (", val, 0), FJ);
		print(data, 2, ") too high, resetting to 1\n", P);
	}
}

//	Print redirections error messages
static void	redirection_msg(t_data *data, int error, char *val)
{
	if (error == OPEN_AMB)
		print(data, 2, ft_strjoin(val, ": ambiguous redirect\n", 0), FP);
	if (error == OPEN_NO_FILE)
		print(data, 2, ft_strjoin(val, ": No such file or directory\n", 0), FP);
	if (error == OPEN_READ)
		print(data, 2, ft_strjoin(val, ": Permission denied\n", 0), FP);
	if (error == OPEN_DIR)
		print(data, 2, ft_strjoin(val, ": is a directory\n", 0), FP);
	if (error == OPEN_FAIL)
		print(data, 2, ft_strjoin(val, ": No such file or directory\n", 0), FP);
	if (error == DUP_FAIL)
		print(data, 2, "error duplicating file descriptor\n", P);
	if (error == PIPE_FAIL)
		print(data, 2, "pipe failed\n", P);
	if (error == SUB_HEREDOC)
	{
		print(data, 2, ft_strjoin("<< ", val, 0), FJ);
		print(data, 2, ": here-document in subshell\n", P);
	}
}

//	Print builtsin error messages
static void	builtsin_msg(t_data *data, int error, char *val)
{
	if (error == CD_PATH)
	{
		print(data, 2, ft_strjoin("cd: ", val, 0), FJ);
		print(data, 2, ": No such file or directory\n", P);
	}
	if (error == CD_HOME)
		print(data, 2, "cd: HOME not set\n", P);
	if (error == CD_OLDPWD)
		print(data, 2, "cd: OLDPWD not set\n", P);
	if (error == CD_ARGS)
		print(data, 2, "cd: too many arguments\n", P);
	if (error == CD_PER)
	{
		print(data, 2, ft_strjoin("cd: ", val, 0), FJ);
		print(data, 2, ": Permission denied\n", P);
	}
	if (error == HIS_ARGS)
		print(data, 2, "history: too many arguments\n", P);
	if (error == HIS_DIG)
	{
		print(data, 2, ft_strjoin("history: ", val, 0), FJ);
		print(data, 2, ": numeric argument required\n", P);
	}
}

//	Print execution error messages
static void	execution_msg(t_data *data, int error, char *val)
{
	if (error == CD_ERROR)
	{
		print(data, 2, ft_strjoin("cd: ", val, 0), FJ);
		print(data, 2, ": No such file or directory\n", P);
	}
	if (error == CMD_LAST)
		print(data, 2, "!!: event not found\n", P);
	if (error == CMD_DIR)
		print(data, 2, ft_strjoin(val, ": is a directory\n", 0), FP);
	if (error == CMD_NO_FILE)
		print(data, 2, ft_strjoin(val, ": command not found\n", 0), FP);
	if (error == CMD_EXEC)
		print(data, 2, ft_strjoin(val, ": Permission denied\n", 0), FP);
	if (error == CMD_FAIL)
		print(data, 2, ft_strjoin(val, ": command failed\n", 0), FP);
	if (error == EXIT_NUM)
	{
		print(data, 2, ft_strjoin("exit: ", val, 0), FJ);
		print(data, 2, ": numeric argument required\n", P);
	}
	if (error == EXIT_ARGS)
		print(data, 2, "exit: too many arguments\n", P);
}

//	Print the error message and exit if required
int	exit_error(t_data *data, int error, int status, char *value)
{
	if (!value)
		value = "";
	print(data, 2, "minishell: ", R);
	catastrophic_msg(data, error, value);
	redirection_msg(data, error, value);
	builtsin_msg(data, error, value);
	execution_msg(data, error, value);
	if (!data)
		return (status % 256);
	if (status >= 1000)
	{
		status -= 1000;
		if (status)
			data->last_status = status;
		data_free(data);
		exit(data->last_status % 256);
	}
	if (status)
		data->last_status = status % 256;
	return (data->last_status);
}
