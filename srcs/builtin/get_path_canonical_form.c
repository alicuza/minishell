#include "minishell.h"

typedef struct s_builder
{
	// int		is_dir;
	size_t	i_orig;
	size_t	i_new;
	size_t	dir_start;
}			t_builder;

static void	skip_dup_slash(char *curpath, size_t len, t_builder *b)
{
	while (b->i_orig < len && curpath[b->i_orig] == '/' && curpath[b->i_orig
		+ 1] == '/')
		b->i_orig++;
}

static void	process_slash(char *curpath, size_t len, t_builder *b)
{
	// b->prev_dir = b->dir_start;
	b->dir_start = b->i_orig;
	skip_dup_slash(curpath, len, b);
}

// static void	move_back(void)

void	init_builder(t_builder *builder)
{
	// builder->is_dir = 0;
	builder->i_orig = 0;
	builder->i_new = 0;
	// builder->prev_dir = -1;
	builder->dir_start = -1;
}

char	*get_path_canonical_form(char *curpath, size_t len)
{
	char		*canonical_form;
	t_builder	b;

	init_builder(&b);
	canonical_form = ft_calloc(sizeof(char), len + 1); ////
	if (canonical_form == NULL)
		return (NULL);
	while (b.i_orig < len && curpath[b.i_orig])
	{
		if (curpath[b.i_orig] == '/')
			process_slash(curpath, len, &b);
		else if (curpath[b.i_orig] == '.')
		{
			if (curpath[b.i_orig + 1] == '/')
			{
				b.i_orig += 2;
				continue ;
				// skip_dup_slash(curpath, len, &b.i_orig);
			}
			else if (curpath[b.i_orig + 1] == '.')
			{
				if (curpath[b.i_orig + 2] == '.')
					return (NULL);
				canonical_form[b.i_new - 1] = 0;
				b.i_new = ft_strrchr(canonical_form, '/') - canonical_form + 1;
				continue ;
			}
		}
		canonical_form[b.i_new] = curpath[b.i_orig];
		b.i_new++;
		b.i_orig++;
	}
	canonical_form[b.i_orig] = '\0';
	return (canonical_form);
}
