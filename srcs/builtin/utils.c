/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaypara <mbaypara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 16:43:59 by mbaypara          #+#    #+#             */
/*   Updated: 2024/10/16 14:23:36 by mbaypara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <unistd.h>
#include <stdio.h>

t_env	*add_env(t_list **g_env, char *key, char *value)
{
	t_list	*env;
	t_env	*content;

	content = check_malloc(ft_calloc(1, sizeof(t_env)));
	env = check_malloc(ft_calloc(1, sizeof(t_list)));
	content->key = check_malloc(ft_strdup(key));
	if (value)
		content->value = check_malloc(ft_strdup(value));
	else
		content->value = check_malloc(ft_strdup(""));
	env->content = content;
	env->next = NULL;
	ft_lstadd_back(g_env, env);
	return (content);
}

int	set_env(t_env *env, char *val)
{
	if (val)
		env->value = check_malloc(ft_strdup(val));
	else
		env->value = check_malloc(ft_strdup(""));
	return (0);
}

void	cd_sync(t_global *g, char *path, char *oldpwd, char *wd)
{
	if (!chdir(path))
	{
		wd = getcwd(NULL, 0);
		if (!wd)
			wd = ft_strdup(env_finder("PWD")->value);
		wd = check_malloc(wd);
		if (!sync_env(&g->env, "OLDPWD", oldpwd)
			|| !sync_env(&g->env, "PWD", wd))
			error_program(ERROR_MALLOC, 12);
		g->error_no = 0;
	}
	else if (ft_strncmp(path, "", 1) != 0)
		return (g->error_no = 1, perror("cd"));
}

t_env	*sync_env(t_list **g_env, char *key, char *value)
{
	t_env	*env;

	if (!value)
		return (NULL);
	env = env_finder(key);
	if (!env)
		env = add_env(g_env, key, value);
	else
		set_env(env, value);
	return (env);
}

int	check_alnum(char *str, int i)
{
	int	j;

	j = -1;
	while (str[++j] && j < i)
		if (!ft_isalnum(str[j]) && str[j] != '_')
			return (0);
	return (1);
}
