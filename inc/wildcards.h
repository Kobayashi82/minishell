/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 19:33:01 by vzurera-          #+#    #+#             */
/*   Updated: 2024/04/28 01:36:06 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WILDCARDS_H
# define WILDCARDS_H

# include <dirent.h>
# include <sys/stat.h>

typedef struct s_wc
{
	char	*input;
	char	*pattern;
	int		input_len;
	int		pattern_len;
	int		match;
	int		start;
	int		i;
	int		j;
}	t_wc;

//	WILDCARDS
bool	is_directory(char *path);
void	tilde(t_data *data, char **cpattern);
void	wildcards(t_data *data, char **cpattern);
void	expand_wildcards(t_data *data, t_args *args);
//	PATTERN
void	dir_get(t_data *data, char *cpattern, char *dir);
//	BRACKETS
bool	brackets(t_wc *wc);

#endif