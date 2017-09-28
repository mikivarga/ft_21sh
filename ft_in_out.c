/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_in_out.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvarga <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/03 17:47:27 by mvarga            #+#    #+#             */
/*   Updated: 2017/08/12 18:35:39 by mvarga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_21sh.h"

int				ft_input(t_term *env)
{
	int			fd;

	if ((fd = open(env->fin, O_RDONLY)) < 0)
		return (FALSE);
	dup2(fd, 0);
	close(fd);
	return (TRUE);
}

int				ft_output(t_term *env)
{
	int			fd;

	if (!ft_strcmp(env->fout, "1") && (env->rout & AGRE))
		dup2(env->fd[1], (fd = 1));
	else if (!ft_strcmp(env->fout, "2") && (env->rout & AGRE))
	{
		dup2((fd = 2), env->fd[1]);
		dup2(env->fd[1], fd);
	}
	else if (!ft_strcmp(env->fout, "-"))
		fd = open("/dev/null", O_WRONLY | O_NDELAY);
	else if ((env->rout & APPEND))
	{
		if ((fd = open(env->fout, O_WRONLY | O_APPEND)) < 0)
			return (FALSE);
	}
	else
	{
		if ((fd = open(env->fout, O_WRONLY | O_CREAT | O_TRUNC, 0666)) < 0)
			return (FALSE);
	}
	dup2(fd, env->fd_agr);
	if (fd != 1)
		close(fd);
	return (TRUE);
}

static char		**ft_redirection_in(char **str, char *c, t_term *cmd)
{
	cmd->fd_agr = 1;
	if (cmd->fin)
		free(cmd->fin);
	if (*c == '<')
	{
		str = ft_heredoc(str, cmd);
		return (str);
	}
	str++;
	if (!*str)
	{
		ft_putendl("21sh: parse error near `\n'");
		cmd->fin = NULL;
	}
	else
	{
		cmd->fin = ft_strdup(*str);
		str++;
	}
	return (str);
}

static char		**ft_redirection_out(char **str, char *c, t_term *cmd)
{
	str++;
	if (cmd->fout)
		free(cmd->fout);
	cmd->rout = 0;
	if (*c == '>')
		cmd->rout |= APPEND;
	if (*c == '&')
	{
		cmd->rout |= AGRE;
		if (*(++c))
			cmd->fout = ft_strdup(c);
		return (str);
	}
	if (!*str)
	{
		ft_putendl("21sh: parse error near `\n'");
		cmd->fout = NULL;
		cmd->rout = 0;
	}
	else
	{
		cmd->fout = ft_strdup(*str);
		str++;
	}
	return (str);
}

char			**ft_redirections(char **str, t_term *cmd)
{
	char		*tmp;
	char		*c;
	char		redir;

	redir = 0;
	if ((c = ft_strchr(*str, '<')))
		redir = *c;
	else if ((c = ft_strchr(*str, '>')))
		redir = *c;
	tmp = c;
	*c++ = '\0';
	if (**str)
	{
		if (*(*str) == '1')
			cmd->fd_agr = 1;
		else if (*(*str) == '2')
			cmd->fd_agr = 2;
	}
	if (redir == '>')
		str = ft_redirection_out(str, c, cmd);
	else if (redir == '<')
		str = ft_redirection_in(str, c, cmd);
	*tmp = redir;
	return (str);
}
