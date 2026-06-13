/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_lex.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sancuta <sancuta@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/12 11:08:25 by sancuta           #+#    #+#             */
/*   Updated: 2026/06/12 11:08:25 by sancuta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_flags(uint32_t flags)
{
	uint32_t	bit;

	if (!flags)
	{
		fprintf(stderr, "(no flags set)");
		return ;
	}
	bit = 1;
	while (!(flags & bit))
		bit <<= 1;
	fprintf(stderr, "%s", get_flag_name(bit));
	flags ^= bit;
	while (flags)
	{
		bit <<= 1;
		if (flags & bit)
		{
			fprintf(stderr, " | %s", get_flag_name(bit));
			flags ^= bit;
		}
	}
}

void	print_lex_state(t_ctx *c, t_lexer_state *l)
{
	unsigned char	ch;

	ch = (unsigned char)c->read_line[l->char_idx];
	fprintf(stderr, "\n--- lex ---\n");
	fprintf(stderr, "  [%lu] ", l->char_idx);
	print_char_info(ch);
	fprintf(stderr, "  %s", get_symbol_type_name(l->type));
	if (l->flags & LEX_IS_BUILDING)
		fprintf(stderr, "(%.*s)", (int)l->token.len,
			c->read_line + l->token.pos);
	fprintf(stderr, "  {  token.pos = %lu  token.len = %lu  flags = ",
		l->token.pos, l->token.len);
	print_flags(l->flags);
	fprintf(stderr, "  }\n");
}

void	print_token(t_ctx *c, t_token *token)
{
	t_arena	*input;

	input = &(c->arena[AT_STRING]);
	fprintf(stderr, "\n--- token ---\n");
	fprintf(stderr, "  %s(", get_symbol_type_name(token->type));
	print_escaped_str(input->buf + token->offset);
	fprintf(stderr, ")  {  offset = %lu  flags = ", token->offset);
	print_flags(token->flags);
	fprintf(stderr, "  }\n");
}
