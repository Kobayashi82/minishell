/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variables.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 19:33:01 by vzurera-          #+#    #+#             */
/*   Updated: 2024/05/11 01:25:11 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VARIABLES_H
# define VARIABLES_H

typedef struct s_data	t_data;
typedef struct s_var
{
	char			*name;
	char			*content;
	bool			ex;
	struct s_var	*prev;
	struct s_var	*next;
}	t_var;

//	SHELL VAR
void	shell_path(t_data *data, char *arg0);
//	EXPAND
int		exval(t_data *data, char **arg, int *i, char *name);
void	expand_vars(t_token *token);
//	EXPAND_QUOTES
bool	export_only(t_data *data, char *name, char *content);
char	*add_quote(t_data *data, char **s);
//	ASSIGN
int		assign_vars(t_data *data, t_token *token);
//	VALIDATE
bool	forbidden_chars(char *new_name, bool unset);
int		validate_var(t_data *data, char *n_var, bool is_env);
//	VARIABLES
bool	is_var(char *arg);
bool	is_vars(t_token *token);
void	set_var(t_data *data, char *name, char *content, bool is_env);
//	ENVIRONMENT
char	*env_get(char **envp, char *name);
void	env_set(char ***envp, char *name, char *content);
void	environment_unset(char ***envp, char *name);
void	*environment_clear(char ***envp);
char	**environment_clone(t_data *data, char **envp);
//	SHELL
t_var	*shell_get(t_var *vars, char *name, bool all);
int		shell_set(t_data *data, t_var **vars, char *name, char *content);
void	shell_unset(t_var **vars, char *name);
void	shell_clear(t_var **vars);
t_var	*shell_clone(t_data *data, t_var *vars);
//	EXTRA
t_var	*join_shell_var(t_var **vars, char *name, char **content);
char	*join_env_var(char **envp, char *name, char *content);

#endif