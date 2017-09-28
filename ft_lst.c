/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lst.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvarga <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/26 11:52:53 by mvarga            #+#    #+#             */
/*   Updated: 2017/08/12 17:09:28 by mvarga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_21sh.h"

void		ft_init_history(t_term *phist)
{
	phist->hist.beg = NULL;
	phist->hist.cur = NULL;
	phist->hist.end = NULL;
	phist->hist.cnt = 0;
}

void		ft_add_history(t_cmd_hist *phist)
{
	t_history		*pnew;

	pnew = (t_history *)malloc(sizeof(t_history));
	if (pnew == NULL)
		exit(EXIT_FAILURE);
	pnew->hist_cmd = (t_leters *)malloc(sizeof(t_leters));
	if (pnew->hist_cmd == NULL)
		exit(EXIT_FAILURE);
	pnew->hist_cmd->beg = NULL;
	pnew->hist_cmd->cur = NULL;
	pnew->hist_cmd->end = NULL;
	pnew->hist_cmd->cnt = 0;
	pnew->next = NULL;
	if (phist->cnt == 0)
	{
		phist->beg = pnew;
		phist->beg->prev = NULL;
	}
	else
	{
		phist->end->next = pnew;
		pnew->prev = phist->end;
	}
	phist->end = pnew;
	phist->cnt++;
}

void		ft_add_letters_end(t_leters *plst, unsigned char c)
{
	t_lst			*pnew;

	pnew = (t_lst *)malloc(sizeof(t_lst));
	if (pnew == NULL)
		exit(EXIT_FAILURE);
	pnew->item = c;
	write(1, &pnew->item, 1);
	pnew->next = NULL;
	if (plst->cnt == 0)
	{
		plst->beg = pnew;
		plst->beg->prev = NULL;
	}
	else
	{
		plst->end->next = pnew;
		pnew->prev = plst->end;
	}
	plst->end = pnew;
	plst->cnt++;
}

void		ft_add_letters_middle(t_leters *plst, unsigned char c)
{
	t_lst			*p;
	t_lst			*temp;

	temp = (t_lst *)malloc(sizeof(t_lst));
	if (temp == NULL)
		exit(EXIT_FAILURE);
	p = plst->cur->next;
	plst->cur->next = temp;
	temp->item = c;
	write(1, &temp->item, 1);
	temp->next = p;
	temp->prev = plst->cur;
	p->prev = temp;
	plst->cnt++;
}

void		ft_let_del(t_term *term)
{
	t_lst			*elm;
	t_leters		*plet;

	plet = term->hist.cur->hist_cmd;
	term->hist.cur->hist_cmd->cur = term->hist.cur->hist_cmd->end;
	if (plet->cur != plet->beg)
	{
		elm = plet->cur;
		if (elm->prev)
			elm->prev->next = elm->next;
		if (elm->next)
			elm->next->prev = elm->prev;
		if (!elm->prev)
			plet->beg = elm->next;
		if (!elm->next)
			plet->end = elm->prev;
		free(elm);
		plet->cnt--;
	}
}
