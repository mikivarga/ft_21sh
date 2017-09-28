/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_show.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvarga <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/18 15:51:33 by mvarga            #+#    #+#             */
/*   Updated: 2017/08/12 18:42:25 by mvarga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_21sh.h"

void				ft_show_prom(t_term *term)
{
	char			*prom;

	prom = ft_get_env("PROM", term);
	tputs(tgoto(tgetstr("ch", NULL), 0, 0), 1, ft_put);
	ft_putstr(prom);
	term->poz.x_prom = ft_strlen(prom);
	free(prom);
	term->poz.x = term->poz.x_prom + 1;
	term->poz.y = 0;
}

static void			ft_show_end(t_leters *plet)
{
	t_lst			*tmp;

	tmp = plet->cur;
	tputs(tgetstr("sc", NULL), 1, ft_put);
	while (tmp)
	{
		write(STDOUT_FILENO, &tmp->item, 1);
		tmp = tmp->next;
	}
	tputs(tgetstr("rc", NULL), 1, ft_put);
}

void				ft_show_str(t_leters *plet, t_term *term)
{
	t_lst			*tmp;

	tmp = plet->beg->next;
	tputs(tgetstr("cd", NULL), 1, ft_put);
	while (tmp != plet->cur)
	{
		if (term->poz.x == term->col)
		{
			tputs(tgetstr("up", NULL), 1, ft_put);
			tputs(tgoto(tgetstr("ch", NULL), 0, term->poz.x), 1, ft_put);
			term->poz.x = 0;
			term->poz.y += 1;
		}
		else if (tmp->item == '\n')
		{
			term->poz.x = 0;
			term->poz.y += 1;
		}
		write(STDOUT_FILENO, &tmp->item, 1);
		term->poz.x += 1;
		if (term->poz.x == term->col && tmp->item != '\n')
			tputs(tgetstr("do", NULL), 1, ft_put);
		tmp = tmp->next;
	}
	ft_show_end(term->hist.cur->hist_cmd);
}

void				ft_show_lst_str(t_leters *plet, t_term *term)
{
	t_lst			*tmp;

	tmp = plet->cur->next;
	tputs(tgetstr("cd", NULL), 1, ft_put);
	tputs(tgetstr("sc", NULL), 1, ft_put);
	while (tmp)
	{
		write(term->fd_tty, &tmp->item, 1);
		tmp = tmp->next;
	}
	tputs(tgetstr("rc", NULL), 1, ft_put);
}

void				ft_show_hist(t_term *term)
{
	term->poz.y = 0;
	term->hist.cur->hist_cmd->cur = term->hist.cur->hist_cmd->beg->next;
	while (term->hist.cur->hist_cmd->cur)
	{
		if (term->poz.x == term->col)
		{
			tputs(tgetstr("up", NULL), 1, ft_put);
			tputs(tgoto(tgetstr("ch", NULL), 0, term->poz.x), 1, ft_put);
			term->poz.x = 0;
			term->poz.y += 1;
		}
		else if (term->hist.cur->hist_cmd->cur->item == '\n')
		{
			term->poz.x = 0;
			term->poz.y += 1;
		}
		write(STDOUT_FILENO, &term->hist.cur->hist_cmd->cur->item, 1);
		term->poz.x += 1;
		term->hist.cur->hist_cmd->cur = term->hist.cur->hist_cmd->cur->next;
		if (term->poz.x == term->col && term->hist.cur->hist_cmd->cur->item \
			!= '\n')
			tputs(tgetstr("do", NULL), 1, ft_put);
	}
	term->hist.cur->hist_cmd->cur = term->hist.cur->hist_cmd->end;
}
