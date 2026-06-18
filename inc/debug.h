/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nribakov <nribakov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/22 21:48:29 by sancuta           #+#    #+#             */
/*   Updated: 2026/06/18 22:08:03 by nribakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEBUG_H
# define DEBUG_H

# include "arena.h"
# include "types.h"

const char	*get_arena_name(t_arena *arena);
const char	*get_flag_name(uint32_t bit);
const char	*get_symbol_type_name(t_symbol_type type);
void		poison_stride(t_arena *arena);
void		print_arena(t_arena *arena);
void		print_flags(uint32_t flags);
void		print_token(t_ctx *c, t_token *token);
void		print_symbol(t_ctx *c, t_symbol *symbol, uint64_t phys);
void		print_lex_state(t_ctx *c, t_lexer_state *l);
void		print_complete_stack(t_ctx *c, t_parser_state *parse);
void		print_char_info(unsigned char ch);
void		print_escaped_str(const char *str);

#endif
