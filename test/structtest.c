#include <stdio.h>

typedef enum {
	e_unkown,
    e_global,
    e_env,
} PointerType;

typedef struct s_list
{

	void			*content;
	struct s_list	*next;
}					t_list;

typedef struct s_env
{
	char		*key;
	char		*value;
}	t_env;

typedef struct s_global
{
	t_list		*garbage_list;
	char		*args;
	t_list		*env;
}	t_global;

void	testfunc(void *ptr)
{
	printf("%d\n", ((t_global *)ptr)->type);
}

int	main(void)
{
	t_env		env;
	t_env		asd;
	t_global	global;
	char		list[] = "merhaba";

	asd.type = e_env;
	env.type = e_env;
	global.type = e_global;
	testfunc(&list);
	if (env.type == asd.type)
		printf("başarılı\n");
	else
		printf("başarısız\n");

    return (0);
}
