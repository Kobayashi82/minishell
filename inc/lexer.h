/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 23:08:17 by vzurera-          #+#    #+#             */
/*   Updated: 2024/05/11 02:13:24 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include <stdbool.h>

enum e_type
{
	CMD,
	LT,
	GT,
	DLT,
	TLT,
	DGT,
	OPAR,
	CPAR,
	PIPE,
	AND,
	OR,
	SMCOL,
	SMAND
};

typedef struct s_data	t_data;
typedef struct s_token	t_token;
typedef struct s_args	t_args;
typedef struct s_redir	t_redir;

typedef struct s_token
{
	int		type;
	char	*cmd;
	char	**args;
	t_args	*args_lst;
	t_redir	*redir;
	t_redir	*s_redir;
	int		fd[2];
	int		pid;
	bool	heredoc;
	bool	redirection;
	bool	wait;
	bool	quoted;
	bool	expand;
	bool	no_exec;
	t_data	*data;
	t_token	*prev;
	t_token	*next;
}	t_token;

typedef struct s_args
{
	char	*arg;
	t_token	*token;
	t_data	*data;
	t_args	*prev;
	t_args	*next;
}	t_args;

typedef struct s_redir
{
	int		type;
	char	*file;
	bool	ambiguous;
	bool	expand;
	t_redir	*prev;
	t_redir	*next;
}	t_redir;

int		check_syntax(t_data *data);
//	LEXER
void	get_token_type(t_token *token, char **s);
int		tokenize(t_data *data);
//	PARSER
int		parser(t_data *data);
//	TOKENS
void	tokens_add(t_data *data, t_token *new_token);
t_token	*tokens_new(t_data *data, char *value, int type);
int		tokens_set(t_data *data);
void	tokens_clear(t_data *data);
int		tokens_add_cmd(t_data *data, char **input);
//	SEPARATORS
int		tokens_add_separator(t_data *data, char **input);
//	ARGS_LIST
void	args_list_clear(t_args **args);
void	args_list_merge(t_args *args, t_args **new_args);
int		reconstruct_args(t_token *token, t_args *args);
//	REDIRECTS
t_token	*next_cmd_token(t_token *token);
int		redirects(t_token *token, bool redirect);
//	REDIR
void	redir_list_clear(t_redir **redir);
t_redir	*get_redir(t_token *token);

#endif
