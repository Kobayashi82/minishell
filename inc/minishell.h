/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 23:08:17 by vzurera-          #+#    #+#             */
/*   Updated: 2024/05/11 02:29:52 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include "colors.h"
# include "terminal.h"
# include "lexer.h"
# include "variables.h"
# include "wildcards.h"
# include "builtsin.h"
# include "executer.h"
# include "clean.h"

typedef struct s_data
{
	int				pid;
	char			**envp;
	char			*input;
	char			quoted;
	char			*prompt;
	int				extra;
	int				last_status;
	int				exit_custom_code;
	bool			reset_status;
	bool			exit;
	int				bk_in;
	int				bk_out;
	int				bk_err;
	int				hd_t;
	int				hd_n;
	char			*msg;
	bool			main;
	bool			is_subshell;
	bool			as_arg;
	struct termios	term;
	time_t			started;
	t_token			*tokens;
	t_token			*last_token;
	t_args			*files;
	t_var			*vars;
}	t_data;

extern int	g_signal;

//	INITIALIZE
void	initialize_subshell(t_data *data, t_data *sub_data, int fd, bool cl_fd);
void	initialize(t_data *data, char **envp, char *arg0);
//	SIGNALS
void	sigquit_handler(int sig);
void	sigint_handler(int sig);
void	sigint_killer(int sig);

#endif