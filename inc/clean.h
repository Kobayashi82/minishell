/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 19:33:01 by vzurera-          #+#    #+#             */
/*   Updated: 2024/05/06 11:04:06 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLEAN_H
# define CLEAN_H

# include <stdbool.h>

enum e_syntax_error
{
	NL,
	PTK,
	TK,
	SOPAR,
	SCPAR,
	QUOTE
};

enum e_redirection_error
{
	OPEN_AMB = 40,
	OPEN_NO_FILE = 41,
	OPEN_READ = 42,
	OPEN_WRITE = 43,
	OPEN_DIR = 44,
	OPEN_FAIL = 45,
	DUP_FAIL = 46,
	PIPE_FAIL = 47,
	SUB_HEREDOC = 48
};

enum e_execution_error
{
	CMD_LAST = 50,
	CMD_NO_FILE = 51,
	CMD_EXEC = 52,
	CMD_DIR = 53,
	CMD_FAIL = 54,
	FORK_FAIL = 55
};

enum e_builtsin_error
{
	CD_PATH = 60,
	CD_ARGS = 61,
	CD_HOME = 62,
	CD_OLDPWD = 63,
	CD_ERROR = 64,
	CD_PER = 65,
	HIS_ARGS = 66,
	HIS_DIG = 67,
	EXIT_NUM = 68,
	EXIT_ARGS = 69
};

enum e_error
{
	NOTHING = 0,
	SYNTAX = 20,
	SHLVL_HIGH = 21,
	VAR_NAME = 30,
	START_ARGS = 70,
	START_BIN = 71,
	START_DIR = 72,
	NO_MEMORY = 90,
	END = 99
};

typedef struct s_data	t_data;
typedef struct s_token	t_token;
typedef struct s_args	t_args;
typedef struct s_redir	t_redir;

//FREE
void	clear_tmp(t_data *data);
void	data_free(t_data *data);
void	free_array(char **array);
//	SAFE
void	*safe_malloc(t_data *data, long bytes);
void	*safe_calloc(t_data *data, int count, long bytes);
void	*safe_strdup(t_data *data, char *str);
int		safe_dup2(t_data *data, int *fd1, int fd2, int close_fd);
//ERROR
int		exit_error(t_data *data, int error, int status, char *value);

#endif
