/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaypara <mbaypara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 11:57:46 by mbaypara          #+#    #+#             */
/*   Updated: 2024/08/25 18:35:04 by mbaypara         ###   ########.fr       */
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
	if (*str)
		printf("%s\n", str);
	clear_garbage(&g->garbage_list);
	system("leaks minishell");
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
		_env = calloc(1, sizeof(t_env));
		if (!_env)
			return (error_program(ERROR_MALLOC, 12), (NULL));
		tmp = ft_split(env[i], '=');
		if (!tmp)
			return (error_program(ERROR_MALLOC, 12), (NULL));
		_env->key = tmp[0];
		_env->value = tmp[1];
		add_list(&g->garbage_list, _env->key);
		add_list(&g->garbage_list, _env->value);
		add_list(&g->garbage_list, _env);
		add_list(&g->garbage_list, tmp);
		ft_lstadd_back(&list, add_list(&g->garbage_list, ft_lstnew(_env)));
		i++;
	}
	return (list);
}
