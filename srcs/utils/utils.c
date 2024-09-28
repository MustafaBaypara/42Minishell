/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaypara <mbaypara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 11:57:46 by mbaypara          #+#    #+#             */
/*   Updated: 2024/09/28 18:11:18 by mbaypara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	*check_malloc(void *ptr)
{
	t_global	*g;

	g = _global(NULL);
	if (!ptr)
		return (error_program(ERROR_MALLOC, 12), NULL);
	return (add_list(&g->garbage_list, ptr));
}

void	error_program(char *str, int err)
{
	t_global	*g;

	g = _global(NULL);
	if (str)
		printf("%s\n", str);
	clear_garbage(&g->garbage_list);
	exit(err);
}

t_list	*env_dup(char **env, t_global *g)
{
	t_list	*list;
	size_t	i;
	t_env	*_env;
	char	**tmp;

	list = NULL;
	i = 0;
	while (env[i])
	{
		_env = check_malloc(ft_calloc(1, sizeof(t_env)));
		tmp = check_malloc(ft_split_first(env[i], '='));
		_env->key = tmp[0];
		_env->value = tmp[1];
		add_list(&g->garbage_list, _env->value);
		add_list(&g->garbage_list, _env->key);
		ft_lstadd_back(&list, check_malloc(ft_lstnew(_env)));
		i++;
	}
	return (list);
}

t_env	*env_finder(char *str)
{
	t_global	*g;
	t_list		*lst;
	t_env		*env;

	g = _global(NULL);
	lst = g->env;
	env = (t_env *)lst->content;

	while (lst)
	{
		if (!ft_strncmp(env->key, str, ft_strlen(str)) && \
		!ft_strncmp(env->key, str, ft_strlen(env->key)))
			return (env);
		lst = lst->next;
		env = (t_env *)lst->content;
	}
	return (NULL);
}
