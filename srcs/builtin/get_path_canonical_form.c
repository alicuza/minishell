#include "minishell.h"
/*
#include "../../libs/libft/libft.h"
#include <stdio.h>
#include <stdlib.h> */

typedef struct s_builder
{
	size_t	i_orig;
	size_t	i_new;
	size_t	dir_start;
}			t_builder;

static void	skip_last_slash(char *canonical_form, t_builder *b)
{
	size_t	len;

	if (b->i_new != 1)
	{
		len = ft_strlen(canonical_form);
		if (canonical_form[len - 1] == '/')
			canonical_form[len - 1] = '\0';
	}
}

static void	process_slash(char *curpath, size_t len, t_builder *b)
{
	b->dir_start = b->i_orig;
	while (b->i_orig < len && curpath[b->i_orig] == '/' && curpath[b->i_orig
		+ 1] == '/')
		b->i_orig++;
}

static int	process_dot(char *curpath, char *canonical_form, t_builder *b)
{
	if (curpath[b->i_orig + 1] == '\0')
		b->i_orig += 1;
	if (curpath[b->i_orig + 1] == '/')
		b->i_orig += 2;
	else if (curpath[b->i_orig + 1] == '.')
	{
		if (curpath[b->i_orig + 2] != '\0' && curpath[b->i_orig + 2] != '/')
			return (EXIT_FAILURE);
		if (b->i_new - 1 != 0)
			canonical_form[b->i_new - 1] = 0;
		b->i_new = ft_strrchr(canonical_form, '/') - canonical_form + 1;
		b->i_orig++;
	}
	else
	{
		canonical_form[b->i_new] = curpath[b->i_orig];
		b->i_new++;
		b->i_orig++;
	}
	return (0);
}

void	init_builder(t_builder *builder)
{
	builder->i_orig = 0;
	builder->i_new = 0;
	builder->dir_start = -1;
}

char	*get_path_canonical_form(char *curpath, size_t len)
{
	char		*canonical_form;
	t_builder	b;

	init_builder(&b);
	canonical_form = ft_calloc(sizeof(char), len + 1);
	if (canonical_form == NULL)
		return (NULL);
	while (b.i_orig < len && curpath[b.i_orig])
	{
		if (curpath[b.i_orig] == '/')
			process_slash(curpath, len, &b);
		else if (curpath[b.i_orig] == '.')
		{
			if (process_dot(curpath, canonical_form, &b) == EXIT_FAILURE)
			{
				free(canonical_form);
				return (NULL);
			}
			continue ;
		}
		canonical_form[b.i_new] = curpath[b.i_orig];
		b.i_new++;
		b.i_orig++;
	}
	canonical_form[b.i_new] = '\0';
	skip_last_slash(canonical_form, &b);
	return (canonical_form);
}

/* int	main(int ac, char *av[])
{
	char	*new;

	new = get_path_canonical_form(av[1], ft_strlen(av[1]));
	printf("Was: %s\n", av[1]);
	printf("New: %s\n", new);
	printf("Comper: %i\n", ft_strncmp(av[2], new, ft_strlen(new)));
	if (ft_strncmp(av[2], new, ft_strlen(new)) != 0)
		printf("KO\n");
	else
		printf("OK\n");
	free(new);
	return (0);
} */
