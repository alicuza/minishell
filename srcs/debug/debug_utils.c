/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sancuta <sancuta@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/22 21:48:29 by sancuta           #+#    #+#             */
/*   Updated: 2026/06/28 05:33:44 by sancuta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

const char	*get_symbol_type_name(t_symbol_type type)
{
	if (type == SYM_NONE)
		return ("SYM_NONE");
	else if (type == SYM_TOKEN)
		return ("SYM_TOKEN");
	else if (type == SYM_OPERATOR)
		return ("SYM_OPERATOR");
	return ("UNKNOWN");
}

const char	*get_flag_name(uint32_t bit)
{
	if (bit == LEX_HAS_QUOTES)
		return ("LEX_HAS_QUOTES");
	else if (bit == LEX_HAS_EXPANSION)
		return ("LEX_HAS_EXPANSION");
	else if (bit == LEX_NEEDS_INPUT)
		return ("LEX_NEEDS_INPUT");
	else if (bit == LEX_IS_BUILDING)
		return ("LEX_IS_BUILDING");
	else if (bit == LEX_IS_DELIMITED)
		return ("LEX_IS_DELIMITED");
	else
		return (NULL);
}

void	print_char_info(unsigned char c)
{
	if (c == '\0')
		fprintf(stderr, "'\\0'(%u)", c);
	else if (c == '\n')
		fprintf(stderr, "'\\n'(%u)", c);
	else if (ft_isspace(c))
		fprintf(stderr, "' '(32)");
	else if (ft_isprint(c))
		fprintf(stderr, "'%c'(%u)", c, c);
	else
		fprintf(stderr, "'.'(%u)", c);
}

void	print_escaped_str(FILE* out, const char *s)
{
	unsigned char	c;

	while (*s)
	{
		c = (unsigned char)*s;
		if (c == '\n')
			fprintf(out, "\\n");
		else if (c == '\\')
			fputc('\\', out);
		else if (ft_isprint(c))
			fputc(c, out);
		else if (ft_isspace(c))
			fputc(' ', out);
		else
			fputc('.', out);
		++s;
	}
}

void	parse_debug_args(int argc, char **argv, t_ctx *c)
{
	size_t	len;
	int		i;

	i = 1;
	while (i < argc)
	{
		len = ft_strlen(argv[i]);
		if (!ft_strncmp(argv[i], "--no_exec", len))
			c->no_exec = true;
		else if (len > 8 && !ft_strncmp(argv[i], "--scope=", 8))
		{
			if (ft_strnstr(argv[i], "tokens", ft_strlen(argv[i])))
				c->scope |= SCOPE_TOKENS;
/*	TODO: to add when appropriate functions have been written and need testing.
 *			if (ft_strnstr(argv[i], "reducer", ft_strlen(argv[i])))
 *				c->scope |= SCOPE_REDUCER;
 *			if (ft_strnstr(argv[i], "stack", ft_strlen(argv[i])))
 *				c->scope |= SCOPE_STACK;
 */			if (!c->scope)
				fprintf(stderr, "--scope: '%s' matched no scope\n", argv[i] + 8);
		}
	++i;
	}
}
