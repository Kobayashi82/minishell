/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 18:52:14 by vzurera-          #+#    #+#             */
/*   Updated: 2024/05/11 02:13:21 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//	Set a char with the value of a token
void	get_token_type(t_token *token, char **s)
{
	*s = "";
	if (token->type == PIPE)
		*s = "|";
	if (token->type == AND)
		*s = "&&";
	if (token->type == OR)
		*s = "||";
	if (token->type == SMCOL)
		*s = ";";
	if (token->type == SMAND)
		*s = "&";
	if (token->type == LT)
		*s = "<";
	if (token->type == DLT)
		*s = "<<";
	if (token->type == TLT)
		*s = "<<<";
	if (token->type == GT)
		*s = ">";
	if (token->type == DGT)
		*s = ">>";
	if (token->type == OPAR)
		*s = "(";
	if (token->type == CPAR)
		*s = ")";
}

//	Clear, process and parse an input to create a token list
int	tokenize(t_data *data)
{
	tokens_clear(data);
	if (tokens_set(data) || parser(data) || check_syntax(data))
	{
		data->extra = -1;
		data->last_token = NULL;
		return (1);
	}
	data->extra = 0;
	return (0);
}
