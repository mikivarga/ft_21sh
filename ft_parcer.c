/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parcer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvarga <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/03 17:12:38 by mvarga            #+#    #+#             */
/*   Updated: 2017/08/12 19:29:39 by mvarga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_21sh.h"

#define IN 1
#define OUT 0

static int		ft_nw(char *str, int (*f)(char))
{
	int			nw;
	int			state;
	char		quote;

	state = OUT;
	nw = 0;
	while (*str)
	{
		if (f(*str))
			state = OUT;
		else if (state == OUT)
		{
			if ((*str == '\'' || *str == '"') && *str)
			{
				quote = *str++;
				while (*str && *str != quote)
					str++;
			}
			state = IN;
			nw++;
		}
		str++;
	}
	return (nw);
}

static int		ft_strlen_arg(char *str, int (*f)(char), int len)
{
	char		quote;

	quote = 0;
	while (*str && !f(*str))
	{
		if (*str == '\'' || *str == '"')
		{
			len += 1;
			quote = *str++;
			while (*str && *str != quote)
			{
				len++;
				str++;
			}
			if (!*(str + 1))
			{
				len++;
				break ;
			}
		}
		len++;
		str++;
	}
	return (len);
}

static char		*ft_new_word(char **pp, char *str, int (*f)(char))
{
	char		*newstr;
	char		*tmp;
	int			len;

	len = 0;
	len = ft_strlen_arg(str, f, len);
	if (!(newstr = (char *)malloc(sizeof(char) * (len + 1))))
		return (NULL);
	tmp = newstr;
	while (len--)
		*tmp++ = *str++;
	*tmp = '\0';
	*pp = newstr;
	return (str);
}

static char		**ft_parse_arg(char *str, int (*f)(char))
{
	char		**pp;
	int			size;
	int			cnt;

	cnt = 0;
	size = ft_nw(str, f);
	if (!(pp = (char **)malloc(sizeof(char *) * (size + 1))))
		return (NULL);
	while (cnt++ < size)
	{
		while (*str && f(*str))
			str++;
		str = ft_new_word(pp++, str, f);
	}
	*pp = 0;
	return (pp - size);
}

int				ft_parser(t_term *cmd, char *buf)
{
	char		**arr;
	char		**pp;

	cmd->arg = ft_parse_arg(buf, ft_new_arg);
	pp = cmd->arg;
	while (*pp)
	{
		if (**pp != '\n')
		{
			arr = ft_parse_arg(*pp, ft_isspace);
			if (!ft_redirection(cmd, arr))
			{
				ft_free(arr);
				ft_free(cmd->arg);
				return (FALSE);
			}
			ft_free(arr);
		}
		pp++;
	}
	ft_free(cmd->arg);
	return (TRUE);
}
