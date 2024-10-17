/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaypara <mbaypara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 14:46:06 by mbaypara          #+#    #+#             */
/*   Updated: 2024/10/17 18:21:56 by mbaypara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <unistd.h>
#include <readline/readline.h>

static char	*ft_strjoin_triple(char *one, char *two, char *three)
{
	char	*res;
	char	*tmp;

	tmp = ft_strjoin(one, two);
	if (!tmp)
		error_program(ERROR_MALLOC, 12);
	res = ft_strjoin(tmp, three);
	if (!res)
		error_program(ERROR_MALLOC, 12);
	free(tmp);
	return (res);
}

char	**list_to_char(t_list	*list)
{
	char	**res;
	t_list	*tmp;
	size_t	i;

	i = 0;
	tmp = list;
	res = check_malloc(ft_calloc(ft_lstsize(list) + 1, sizeof(char *)));
	while (tmp)
	{
		res[i] = check_malloc(ft_strjoin_triple((((t_env *)tmp->content)->key),
					"=", (((t_env *)tmp->content)->value)));
		tmp = tmp->next;
		i++;
	}
	res[i] = NULL;
	return (res);
}

t_global	*_global(t_global *g)
{
	static t_global	*global;

	if (g)
		global = g;
	return (global);
}

void	init_global(t_global *g)
{
	_global(g);
	g->garbage_list = NULL;
	g->single_quotes = 0;
	g->double_quotes = 0;
	g->token_list = NULL;
	g->cmd_list = NULL;
	g->path = NULL;
	g->control = 1;
	g->error_no = 0;
	catch_signal(1);
}

int	main(int ac, char **av, char **env)
{
	t_global	global;
	char		**envtwo;

	init_global(&global);
	if (ac != 1 || av[1])
		error_program(ERROR_ARG, 1);
	if (!(*env))
	{
		envtwo = check_malloc(ft_calloc(2, sizeof(char *)));
		envtwo[0] = getcwd(NULL, 0);
		if (!envtwo[0])
			envtwo[0] = ft_strdup("PWD= ");
		else
			envtwo[0] = ft_strjoin(check_malloc(ft_strdup("PWD=")), envtwo[0]);
		envtwo[0] = check_malloc(envtwo[0]);
		global.env = env_dup(envtwo);
	}
	else
		global.env = env_dup(env);
	global.the_env = list_to_char(global.env);
	loop(&global);
	rl_clear_history();
	clear_garbage(&global.garbage_list);
	return (global.error_no);
}
