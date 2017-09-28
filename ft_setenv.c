/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_setenv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvarga <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/25 15:41:26 by mvarga            #+#    #+#             */
/*   Updated: 2017/06/25 16:22:19 by mvarga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_21sh.h"

static char		*ft_value(char *value)
{
	int			len;
	char		*tmp;
	char		*str;
	char		quote;

	quote = 0;
	if (*value == '"' || *value == '\'')
		quote = *value++;
	len = ft_strlen(value);
	if (!(str = (char *)malloc(sizeof(char) * (len + 1))))
		return (NULL);
	tmp = str;
	while (*value && *value != quote)
		*tmp++ = *value++;
	*tmp = '\0';
	return (str);
}

static char		*ft_add_new_strenv(char *name, char *value)
{
	char		*newstr;
	int			len;
	char		*tmp;

	tmp = NULL;
	len = ft_strlen(name) + 1 + ft_strlen(value);
	if ((newstr = (char *)malloc(sizeof(char) * (len + 1))))
	{
		ft_strcpy(newstr, name);
		ft_strcat(newstr, "=");
		tmp = ft_value(value);
		ft_strcat(newstr, tmp);
		if (tmp)
			free(tmp);
	}
	return (newstr);
}

int				ft_envname_cmp(char *name, char *envstr)
{
	char		*p;
	int			check;

	if ((p = ft_strchr(envstr, '=')) == NULL)
		return (FALSE);
	*p = '\0';
	check = ft_strcmp(name, envstr);
	*p = '=';
	return (check == 0 ? TRUE : FALSE);
}

static int		ft_check_value(char *name, char *value, t_term *cmd)
{
	char		**pp;
	int			flag;

	if (!name || !value)
	{
		ft_putendl("usage: setenv [env] <value>");
		return (TRUE);
	}
	flag = FALSE;
	pp = cmd->env;
	while (*pp)
	{
		if (ft_envname_cmp(name, *pp))
		{
			free(*pp);
			*pp = ft_add_new_strenv(name, value);
			flag = TRUE;
		}
		pp++;
	}
	return (flag);
}

void			ft_setenv(char *name, char *value, t_term *cmd)
{
	char		**newenv;
	int			cnt;
	int			i;

	if (ft_check_value(name, value, cmd))
		return ;
	cnt = 1;
	i = 0;
	while (cmd->env[i++])
		cnt++;
	if ((newenv = (char **)malloc(sizeof(char *) * (cnt + 1))))
	{
		i = 0;
		while (i < cnt + 1)
			newenv[i++] = NULL;
		i = 0;
		while (cmd->env[i])
		{
			newenv[i] = ft_strdup(cmd->env[i]);
			free(cmd->env[i++]);
		}
		newenv[i] = ft_add_new_strenv(name, value);
		free(cmd->env);
		cmd->env = newenv;
	}
}
