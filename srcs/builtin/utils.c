/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaypara <mbaypara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 16:43:59 by mbaypara          #+#    #+#             */
/*   Updated: 2024/10/09 16:57:49 by mbaypara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static t_list	*add_env(t_list **g_env, char *key, char *value)
{
	t_list	*env;
	t_env	*content;

	content = check_flag(ft_calloc(1, sizeof(t_env)));
	env = check_malloc(ft_calloc(1, sizeof(t_list)));
	
	content->key = check_malloc(ft_strdup(key));
	if (value)
		content->value = check_malloc(ft_strdup(value));
	else
		content->value = check_malloc(ft_strdup(""));
	env->content = content;
	env->next = NULL;
	ft_lstadd_back(g_env, env);
	return (env);
}

t_list	*sync_env(t_list **g_env, char *key, char *value)
{
	t_list	*env;
	if (!value)
		return (NULL);
	env = env_finder(key);
	if (!env)
	{
		env = add_env(g_env, key, value);
		if (!env)
			return (NULL);
	}
}
