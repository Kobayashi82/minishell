/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtsin.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 19:04:29 by vzurera-          #+#    #+#             */
/*   Updated: 2024/05/07 22:49:53 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTSIN_H
# define BUILTSIN_H

# include <stdbool.h>

typedef struct s_data	t_data;
typedef struct s_token	t_token;

//	BUILTSIN
bool		check_builtin(t_token *token);
bool		is_builtin(char *arg0);
int			builtsin(t_token *token);
//	ECHO
int			echo(t_token *token, char **args);
//	CD
int			cd(t_token *token, char **args);
//	PWD
int			pwd(t_token *token, char **args);
//	ENV
int			env(t_data *data, char **args);
//	EXPORT
int			export(t_token *token, char **args);
void		export_sort(t_data *data, char **envp, int is_p);
void		export_print(t_data *data, char **envp, int is_p);
//	UNSET
int			unset(t_data *data, char **args);
//	HISTORY
int			history(t_token *token, char **args);
HIST_ENTRY	**copy_history(void);
void		restore_history(HIST_ENTRY **old_history);
//	EXIT
int			exit_minishell(t_token *token, char **args);
//	ABOUT
int			about(t_data *d);
//	HELP
int			help(t_data *d);

#endif