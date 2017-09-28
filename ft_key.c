/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_key.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvarga <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/19 12:26:05 by mvarga            #+#    #+#             */
/*   Updated: 2017/08/12 17:53:35 by mvarga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_21sh.h"

void				ft_key_up_dw(t_term *term, unsigned long key)
{
	if (key == KEY_UP)
	{
		if (!term->hist.cur->prev)
			return ;
		term->poz.x = term->poz.x_prom + 1;
		tputs(tgoto(tgetstr("ch", NULL), 0, term->poz.x), 1, ft_put);
		while (term->poz.y--)
			tputs(tgetstr("up", NULL), 1, ft_put);
		tputs(tgetstr("cd", NULL), 1, ft_put);
		term->hist.cur = term->hist.cur->prev;
		ft_show_hist(term);
	}
	else if (key == KEY_DWN)
	{
		if (!term->hist.cur->next)
			return ;
		term->poz.x = term->poz.x_prom + 1;
		tputs(tgoto(tgetstr("ch", NULL), 0, term->poz.x), 1, ft_put);
		while (term->poz.y--)
			tputs(tgetstr("up", NULL), 1, ft_put);
		tputs(tgetstr("cd", NULL), 1, ft_put);
		term->hist.cur = term->hist.cur->next;
		ft_show_hist(term);
	}
}

void				ft_key_lft(t_term *term)
{
	t_leters		*plet;

	plet = term->hist.cur->hist_cmd;
	if (!plet->cur->prev)
		return ;
	if (term->poz.x == 1 && term->hist.cur->hist_cmd->cur->item != '\n')
	{
		term->poz.x = term->col;
		term->poz.y -= 1;
		tputs(tgetstr("le", NULL), 1, ft_put);
	}
	else if (term->hist.cur->hist_cmd->cur->item == '\n')
	{
		term->poz.x = term->poz.x_prom + 1;
		tputs(tgoto(tgetstr("ch", NULL), 0, term->poz.x), 1, ft_put);
		while (term->poz.y--)
			tputs(tgetstr("up", NULL), 1, ft_put);
		term->poz.y = 0;
		ft_show_str(term->hist.cur->hist_cmd, term);
	}
	else
		ft_key_l(term);
	plet->cur = plet->cur->prev;
}

void				ft_key_rght(t_term *term)
{
	t_leters		*plet;

	plet = term->hist.cur->hist_cmd;
	if (!plet->cur->next)
		return ;
	if (term->hist.cur->hist_cmd->cur->next->item == '\n')
	{
		ft_poz_x_y(term);
		tputs(tgoto(tgetstr("ch", NULL), 0, 0), 1, ft_put);
		tputs(tgetstr("do", NULL), 1, ft_put);
	}
	else if ((term->poz.x + 1) == term->col)
	{
		tputs(tgoto(tgetstr("ch", NULL), 0, 0), 1, ft_put);
		tputs(tgetstr("do", NULL), 1, ft_put);
		term->poz.x += 1;
	}
	else
		ft_key_r(term);
	plet->cur = plet->cur->next;
}

static void			ft_del_let(t_leters *plet, t_lst *elm)
{
	if (elm->prev)
		elm->prev->next = elm->next;
	if (elm->next)
		elm->next->prev = elm->prev;
	if (!elm->prev)
		plet->beg = elm->next;
	if (!elm->next)
		plet->end = elm->prev;
}

void				ft_key_del(t_term *term)
{
	t_lst		*elm;
	t_leters	*plet;

	plet = term->hist.cur->hist_cmd;
	if (plet->cur == plet->beg || !plet->cur->prev)
		return ;
	if (term->hist.cur->hist_cmd->cur->item == '\n')
	{
		ft_key_lft(term);
		term->hist.cur->hist_cmd->cur = term->hist.cur->hist_cmd->cur->next;
	}
	elm = plet->cur;
	ft_del_let(plet, elm);
	if (elm->item == '\n')
		ft_key_rght(term);
	ft_key_lft(term);
	free(elm);
	plet->cnt--;
	ft_show_lst_str(plet, term);
}
