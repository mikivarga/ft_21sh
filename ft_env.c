/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvarga <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/25 15:43:33 by mvarga            #+#    #+#             */
/*   Updated: 2017/07/18 19:10:42 by mvarga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_21sh.h"

static int		ft_strlen_env(char **env)
{
	int			cnt;

	cnt = 0;
	while (*env++)
		cnt++;
	return (cnt);
}

char			*ft_get_env(char *name, t_term *env)
{
	char		*str;
	char		**pp;
	char		*tmp;
	int			len;

	str = NULL;
	pp = env->env;
	while (*pp)
	{
		if (ft_envname_cmp(name, *pp))
		{
			tmp = ft_strchr(*pp, '=');
			len = ft_strlen(++tmp);
			if (!(str = (char *)malloc(sizeof(char) * (len + 1))))
				return (NULL);
			ft_strcpy(str, tmp);
		}
		pp++;
	}
	return (str);
}

void			ft_new_env(t_term *cmd, char **env)
{
	char		**pp;
	int			len;

	len = ft_strlen_env(env);
	if ((pp = (char **)malloc(sizeof(char *) * (len + 1))))
	{
		cmd->env = pp;
		while (*env)
			*pp++ = ft_strdup(*env++);
		*pp = NULL;
	}
	else
		cmd->env = NULL;
}
