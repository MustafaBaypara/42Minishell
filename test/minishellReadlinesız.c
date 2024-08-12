/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishellReadlinesız.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaypara <mbaypara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 14:46:06 by mbaypara          #+#    #+#             */
/*   Updated: 2024/07/03 20:50:22 by mbaypara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "stdio.h"

int	main(int ac, char **av, char **env)
{
	t_list	*list;
	char				*input;
	char	**txt;

	(void)ac;
	(void)av;
	(void)env;
	list = NULL;
	int i = 0;
	txt = ft_split("merhaba nasılsın ben buradayım", ' ');
	while (1)
	{
		// input = readline("minishell> ");
		input = txt[i++];
		if (!input)
			break ;
		printf("%s\n", input);
		add_garbage(&list, input);
		if (!ft_strncmp(input, "bitir", ft_strlen(input)))
			break ;
	}
	printf("*------*\n%s\n", list->ptr);
	printf("*------*\n%s\n", list->next->ptr);
	printf("*------*\n%s\n", list->next->next->ptr);
	printf("*------*\n%s\n", list->next->next->next->ptr);
	clear_garbage(&list);
	free(txt);
}
