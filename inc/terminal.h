/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 19:33:01 by vzurera-          #+#    #+#             */
/*   Updated: 2024/05/11 01:36:17 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TERMINAL_H
# define TERMINAL_H

# include <readline/readline.h>
# include <readline/history.h>
# include <sys/wait.h>
# include <time.h>
# include <fcntl.h>
# include <stdio.h>
# include <unistd.h>
# include <signal.h>
# include <sys/ioctl.h>
# include <termios.h>
# include <errno.h>
# include <stdbool.h>

enum e_print
{
	R,
	RP,
	FR,
	FRP,
	J,
	FJ,
	P,
	FP
};

typedef struct s_data	t_data;
typedef struct s_token	t_token;
typedef struct s_trim
{
	t_token	*token;
	char	*copy;
	char	*arg;
	int		i;
	int		j;
	bool	convert;
	bool	quoted;
}	t_trim;

//	BANNER
void	print_banner(t_data *d);
void	print_welcome(t_data *d);
void	first_start(t_data *data);
//	PROMPT
void	set_prompt(t_data *data);
int		print(t_data *data, int fd, char *s, int mode);
//	INPUT
void	process_input(t_data *data);
int		read_input(t_data *data);
void	argument_error(t_data *data, char *arg);
//	SUB_INPUT
void	clean_comments(char *line);
int		open_quote(t_data *data, char *str, bool reset);
int		open_pars(char *input, bool reset);
bool	open_line(t_data *data, char *input);
void	read_sub_input(t_data *data, char **input);
//	QUOTES
bool	skip_quotes(char *input, int *pos);
bool	skip_paren(char *input, int *i);
//	ESCAPES
char	*escape_input(char *str);
int		trim_esc(t_token *token, char *arg, bool convert, bool quoted);
char	escape_chars(char c);
bool	is_esc(char *str, int end);
void	trim_spaces(char *s1);
//	LAST_CMD
int		check_last_cmd(t_data *data);
void	set_last_arg(t_token *token);

#endif