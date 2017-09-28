/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_heredoc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvarga <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/08 19:14:33 by mvarga            #+#    #+#             */
/*   Updated: 2017/08/12 17:04:34 by mvarga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_21sh.h"

static void		ft_beg(t_term *term)
{
	t_leters	*tmp;

	tmp = term->hist.cur->hist_cmd;
	while (tmp->cur->prev)
		tmp->cur = tmp->cur->prev;
}

static int		ft_check_redir(char *str, t_term *term)
{
	t_leters	*tmp;

	tmp = term->hist.cur->hist_cmd;
	while (tmp->cur->prev)
		tmp->cur = tmp->cur->prev;
	tmp->cur = tmp->cur->next;
	while (tmp->cur->next)
	{
		if (*str && *str == tmp->cur->item)
			str++;
		else
			return (FALSE);
		tmp->cur = tmp->cur->next;
	}
	if (!*str && !tmp->cur->next)
		return (TRUE);
	return (FALSE);
}

static void		ft_loop(char *str, t_term *term)
{
	t_lst		*p;

	term->hist.cur->hist_cmd->cur = term->hist.cur->hist_cmd->end;
	ft_putstr("heredoc> ");
	p = term->hist.cur->hist_cmd->end->prev;
	term->hist.cur->hist_cmd->cur = term->hist.cur->hist_cmd->end;
	term->hist.cur->hist_cmd->cur->prev = NULL;
	ft_read_input(term);
	if (!ft_check_redir(str, term))
		ft_loop(str, term);
	ft_beg(term);
	term->hist.cur->hist_cmd->cur->prev = p;
}

static void		ft_save(char *str, t_term *term)
{
	t_leters	*tmp;
	int			fd;
	t_lst		*p;
	int			cnt;

	cnt = 0;
	fd = open(term->fin, O_WRONLY | O_APPEND);
	tmp = term->hist.cur->hist_cmd;
	p = tmp->cur;
	while (p->next)
	{
		cnt++;
		p = p->next;
	}
	cnt -= ft_strlen(str);
	while (cnt--)
	{
		write(fd, &(tmp->cur->item), 1);
		tmp->cur = tmp->cur->next;
	}
	close(fd);
}

char			**ft_heredoc(char **str, t_term *term)
{
	int			fd;

	str++;
	if (!*str)
	{
		ft_putendl("21sh: parse error near `\n'");
		term->fin = NULL;
		return (str);
	}
	fd = open("tmp", O_WRONLY | O_CREAT | O_TRUNC, 0666);
	close(fd);
	term->fin = ft_strdup("tmp");
	ft_open_visual_mode(term);
	term->hist.cur->hist_cmd->cur = term->hist.cur->hist_cmd->end;
	ft_add_let(term, '\n');
	tputs(tgetstr("up", NULL), 1, ft_put);
	ft_loop(*str, term);
	term->hist.cur->hist_cmd->cur = term->hist.cur->hist_cmd->cur->next;
	ft_save(*str, term);
	ft_close_visual_mode(term);
	return (++str);
}
