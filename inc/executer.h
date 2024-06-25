/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 19:04:29 by vzurera-          #+#    #+#             */
/*   Updated: 2024/04/30 14:18:32 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTER_H
# define EXECUTER_H

# include <stdbool.h>

typedef struct s_data	t_data;
typedef struct s_subshell
{
	char	*output;
	int		status;
	bool	expand;
	bool	subshell;
}	t_subshell;

//	PRE_EXECUTER
t_token	*next_separator(t_token *token, int type);
void	pre_execute(t_data *data, int status);
//	EXECUTER
bool	is_pipe(t_token *token);
void	close_fd(int *fd);
int		close_fds(t_token *token, int mode);
int		expand(t_token *token, bool is_last_arg);
int		execute(t_token *token);
//	SUB_SHELL_PAR
bool	check_no_exec(t_token *token);
t_token	*prev_cmd(t_token *token);
bool	is_parenthesis(t_token *token);
//	SUB_SHELL_VAR
void	pre_subshell_var(t_data *data, char **arg);
//	FORKED
int		forking(t_token *token);
//	FORKED_SUBSHELL
void	subshell_par_child(t_data *data, t_data *sub_data, t_token *token);
//	REDIRECTIONS
void	reset_redirections(t_data *data, bool in, bool out, bool err);
char	*get_output(t_data *data, int fd);
int		set_redirections(t_token *token, bool forked);
//	HEREDOC
void	expand_heredoc(t_data *data, char **text);
void	heredocs(t_data *data);
//	HERESTRINGS
char	*ft_replace(char *str, int *start, int len, char *replace);
char	*generate_tmp_name(t_data *data);
int		herestring(t_data *data, t_redir **redir, bool forked);
//	EXPAND_REDIRECTIONS
int		expand_closer(int fd, char *text);
int		expand_input(t_data *data, t_redir *redir);
//	BUILDPATH
char	*create_path(t_data *data, char *arg0);
int		trim_arg(t_token *token, char *arg);
int		build_path(t_data *data, t_token *token);

#endif