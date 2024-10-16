/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaypara <mbaypara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 17:44:06 by mbaypara          #+#    #+#             */
/*   Updated: 2024/10/16 13:30:32 by mbaypara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	put_env(t_list *lst, int fd)
{
	t_env	*env;

	env = (t_env *)lst->content;
	while (lst)
	{
		if (env->value && env->key)
		{
			ft_putstr_fd(env->key, fd);
			ft_putstr_fd("=", fd);
			ft_putendl_fd(env->value, fd);
		}
		lst = lst->next;
		if (lst)
			env = (t_env *)lst->content;
	}
}

int	env(t_command *c, t_global *g)
{
	if (!check_flag(c))
		return (g->error_no = 1, 1);
	if (c->value[1])
	{
		ft_putendl_fd("minishell: env: too many arguments", 2);
		return (g->error_no = 1, 1);
	}
	put_env(g->env, c->fd[1]);
	g->error_no = 0;
	return (1);
}
