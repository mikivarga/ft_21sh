/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_redirection.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvarga <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/03 17:30:38 by mvarga            #+#    #+#             */
/*   Updated: 2017/08/12 18:33:26 by mvarga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_21sh.h"

static int		ft_nw(char **str)
{
	int			i;

	i = 0;
	while (str[++i])
		;
	return (i);
}

static char		*ft_expending_cmd(char *tmp, char *str, int *i, t_term *env)
{
	char		*tmp_env;
	char		env_name[20];
	int			j;

	j = 0;
	str++;
	while (*str && !ft_isspace(*str) && *str != '"' && *str != '\'')
		env_name[j++] = *str++;
	env_name[j] = '\0';
	j = 0;
	if ((tmp_env = ft_get_env(env_name, env)))
	{
		while (tmp_env[j])
			tmp[(*i)++] = tmp_env[j++];
		free(tmp_env);
	}
	return (str);
}

static void		ft_new_word(char **pp, char *str, int *i, t_term *env)
{
	char		tmp[BUFSIZ];
	char		quote;

	*i = 0;
	while (*str)
	{
		if (*str == '\'' || *str == '"')
		{
			quote = *str++;
			while (*str && *str != quote)
			{
				if (*str == '$' && *(str - 1) != '\'')
					str = ft_expending_cmd(tmp, str, i, env);
				else
					tmp[(*i)++] = *str++;
			}
			str++;
		}
		if (*str && *str == '$')
			str = ft_expending_cmd(tmp, str, i, env);
		else if (*str)
			tmp[(*i)++] = *str++;
	}
	tmp[*i] = '\0';
	*pp = ft_strdup(tmp);
}

static char		**ft_parce_redirection(char **str, t_term *cmd, int i, int size)
{
	char		**p;
	char		**pp;

	size = ft_nw(str);
	if (!(pp = (char **)malloc(sizeof(char *) * (size + 1))))
		return (NULL);
	p = pp;
	if (size == 1 && !ft_strchr(*str, '<') && !ft_strchr(*str, '>'))
		ft_new_word(pp++, *str++, &i, cmd);
	while (*str)
	{
		if (*str && (ft_strchr(*str, '<') || ft_strchr(*str, '>')))
			str = ft_redirections(str, cmd);
		else if (*str)
			ft_new_word(pp++, *str++, &i, cmd);
	}
	*pp = 0;
	return (p);
}

int				ft_redirection(t_term *env, char **arr)
{
	char		**pp;
	int			flag;
	int			i;
	int			size;

	env->fd_agr = 1;
	size = 0;
	i = 0;
	if (env->fin)
		free(env->fin);
	env->fin = NULL;
	if (env->fout)
		free(env->fout);
	env->fout = NULL;
	env->rout = 0;
	pp = ft_parce_redirection(arr, env, i, size);
	if (*pp)
		flag = ft_parse_pipe(env, pp);
	else
		return (TRUE);
	if (flag == WARNING)
		flag = ft_select_cmd(pp, env, TRUE);
	ft_free(pp);
	return (flag);
}
