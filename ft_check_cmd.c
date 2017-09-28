/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_check_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvarga <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/20 19:11:18 by mvarga            #+#    #+#             */
/*   Updated: 2017/08/12 16:49:24 by mvarga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_21sh.h"

static char		*ft_check_quote(char *str, t_term *term)
{
	char		quote;

	quote = 0;
	if (*str && (*str == '\'' || *str == '"'))
	{
		quote = *str++;
		while (*str && *str != quote)
			str++;
		if (!*str)
		{
			if (quote == '\'')
				ft_putstr("quote> ");
			else if (quote == '\"')
				ft_putstr("dquote> ");
			ft_read_input(term);
		}
		else
			str++;
	}
	return (str);
}

static int		ft_err_cmd(char *str, int fl)
{
	if (*str && ((*str == ';' && !fl) || (*str == ';' && *(str - 1) == ';')))
	{
		if (*str == ';' && *(str + 1) == ';')
			ft_putendl("21sh : parse error near \';;\'");
		else if (*str == ';' && *(str - 1) == ';')
			ft_putendl("21sh : parse error near \';;\'");
		else
			ft_putendl("21sh : parse error near \';\'");
		return (TRUE);
	}
	return (FALSE);
}

int				ft_check_cmd(char *str, int fl, t_term *term)
{
	while (*str)
	{
		fl = 1;
		if (*str && *str == '\\')
			str += 2;
		str = ft_check_quote(str, term);
		if (*str && *str == ';')
		{
			str++;
			while (*str && ft_isspace(*str))
			{
				fl = 0;
				str++;
			}
			if (!*str)
				break ;
			if (*str && ft_err_cmd(str, fl))
				return (FALSE);
			str--;
		}
		if (*str && *str != '\'' && *str != '\"')
			str++;
	}
	return (TRUE);
}
