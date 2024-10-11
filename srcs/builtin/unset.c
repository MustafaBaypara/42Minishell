/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaypara <mbaypara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 17:04:34 by mbaypara          #+#    #+#             */
/*   Updated: 2024/10/11 18:02:02 by mbaypara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	export_del(t_list **lst, char *str)
{
	t_list	*tmp;
	t_list	*prev;
	t_env	*env;

	tmp = *lst;
	prev = NULL;
	if (!tmp)
		return (1);
	env = (t_env *)tmp->content;
	if (ft_strncmp(env->key, str, ft_strlen(str)) == 0)
		return (*lst = tmp->next, 1);
	while (tmp)
	{
		env = (t_env *)tmp->content;
		if (ft_strncmp(env->key, str, ft_strlen(str)) == 0)
		{
			if (prev)
				prev->next = tmp->next;
			return (1);
		}
		prev = tmp;
		tmp = tmp->next;
	}
	return (1);
}

int	unset(t_command *cmd, t_global *g)
{
	int	i;

	i = 0;
	if (!check_flag(cmd))
		return (g->error_no = 1, 1);
	while (cmd->value[++i])
		if (export_del(&g->env, cmd->value[i]) == 0)
			return (0);
	return (g->error_no = 0, 1);
}
