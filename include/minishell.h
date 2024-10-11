/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaypara <mbaypara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 14:46:15 by mbaypara          #+#    #+#             */
/*   Updated: 2024/10/11 17:59:29 by mbaypara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "unistd.h"
# include "../srcs/libft/libft.h"
# include "shellutils.h"
# include "errorlib.h"

void			*add_list(t_list **list, void *garbage);
void			clear_garbage(t_list **lst);
void			clean_list(t_list **list);
void			remove_from_list(t_list **list, void *dirty);
void			loop(t_global *g);
t_list			*env_dup(char **env);
void			error_program(char *str, int err);
t_global		*_global(t_global *g);
void			*check_malloc(void *ptr);
char			**check_malloc_split(char **split_result);
char			**ft_split_first(const char *s, char c);
t_token_types	identifier(char *token);
int				is_white_space(char c);
char			*quote_clean(char *str, int s, int d);
t_env			*env_finder(char *str);
char			*dollar_sign(char *tmp, char *t_val, size_t *i, t_global *g);
int				check_hdoc(t_command *cmd);
char			**list_to_char(t_list	*list);

// LEXER
void			lexer(t_global *g);
int				check_syntax(t_global *g);
size_t			check_space(char *str);

// PARSER
void			parser(t_global *g);
size_t			token_len(t_list *list);
size_t			word_end(const char *v, size_t start, int *d, int *s);
void			toggle_quote(char c, int *in_s, int *in_d);
int				rdr_position(t_command *cmds);

// EXPANDER
void			expander(t_global *g);
int				dollar(t_global	*g, char **value);
int				home(t_global *g, char **value, size_t i, char *tmp);
void            remove_empty_elements(char **arr);

// HEREDOC
int				heredocs(t_global *g, t_command *cmd);

// SIGNALS
void			catch_signal(int num);

// EXECUTOR
void			executor(t_global *g);
void			wait_func(t_global *g, t_command *cmd);
int				files(t_command *cmd, size_t *i, int fd);
int				check_slash(char *val);
void			catch_error(t_command *cmd, int i, t_global *g);
int				is_command_ok(t_command *cmd, t_global *g);
void			prepare_next(t_global *g);
void			close_fds(t_command *cmd, int i);

// BUILTIN
int				builtin_check(t_command *cmd, int num);
int				echo(t_command *c, t_global *g);
int				check_flag(t_command *c);
int				env(t_command *c, t_global *g);
int				exit_func(t_command *cmd, t_global *g);
int				cd(t_command *cmd, t_global *g);
t_env			*sync_env(t_list **g_env, char *key, char *value);
void			cd_sync(t_global *g, char *path, char *oldpwd, char *wd);
int				pwd(t_command *cmd, t_global *g);
int				check_alnum(char *str, int i);
int				set_env(t_env *env, char *val);
t_env			*add_env(t_list **g_env, char *key, char *value);
t_list			*msh_lstcpy(t_list *lst);
int				export(t_command *cmd, t_global *g);
void			msh_lstsort(t_list **lst);
int				unset(t_command *cmd, t_global *g);

#endif