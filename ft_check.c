/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_check.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvarga <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/11 16:43:39 by mvarga            #+#    #+#             */
/*   Updated: 2017/08/12 16:47:34 by mvarga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_21sh.h"

static int		ft_check_basic_cmd(char *str, t_term *term)
{
	int			fl;

	fl = 1;
	while (*str && ft_isspace(*str))
		str++;
	if (*str)
	{
		if (*str == ';')
		{
			if (*str == ';' && *(str + 1) == ';')
				ft_putendl("21sh : parse error near \';;\'");
			else
				ft_putendl("21sh : parse error near \';\'");
			return (FALSE);
		}
		if (ft_check_cmd(str, fl, term))
			return (TRUE);
	}
	return (FALSE);
}

static void		ft_beg(t_term *term)
{
	t_leters	*tmp;

	tmp = term->hist.cur->hist_cmd;
	while (tmp->cur->prev)
		tmp->cur = tmp->cur->prev;
}

static void		ft_newline(t_term *term)
{
	ft_putstr("> ");
	ft_read_input(term);
}

int				ft_check(t_term *term)
{
	char		buf[BUF];
	t_lst		*p;

	term->hist.cur->hist_cmd->cur = term->hist.cur->hist_cmd->end;
	ft_add_let(term, '\n');
	ft_save_buf(term, buf);
	p = term->hist.cur->hist_cmd->end->prev;
	term->hist.cur->hist_cmd->cur = term->hist.cur->hist_cmd->end;
	term->hist.cur->hist_cmd->cur->prev = NULL;
	if (p->item == '\\')
		ft_newline(term);
	else
	{
		if (!ft_check_basic_cmd(buf, term))
		{
			ft_beg(term);
			term->hist.cur->hist_cmd->cur->prev = p;
			return (FALSE);
		}
	}
	ft_beg(term);
	term->hist.cur->hist_cmd->cur->prev = p;
	return (TRUE);
}
