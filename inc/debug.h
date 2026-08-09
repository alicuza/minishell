/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nribakov <nribakov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/22 21:48:29 by sancuta           #+#    #+#             */
/*   Updated: 2026/07/24 14:59:43 by nribakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEBUG_H
# define DEBUG_H

# include "arena.h"
# include "types.h"

/* -------- debug_arena.c -------------------------------------------------- */
void		poison_stride(t_arena *arena);
const char	*get_arena_name(t_arena *arena);
void		print_arena(t_arena *arena);

/* -------- debug_lex.c ---------------------------------------------------- */
void		print_flags(FILE *out, uint32_t flags);
void		print_lex_state(t_ctx *c, t_lexer_state *l);
void		print_token(FILE *out, t_ctx *c, t_token *token);
void		print_token_line(FILE *out, t_ctx *c, t_token *token);

/* -------- debug_names.c -------------------------------------------------- */
const char	*get_token_type_name(t_token_type type);
const char	*get_symbol_type_name(t_symbol_type type);
const char	*get_node_type_name(t_node_type type);
const char	*get_node_flag_name(uint32_t bit);
const char	*get_lex_flag_name(uint32_t bit);
const char	*get_parse_flag_name(uint32_t bit);

/* -------- debug_parse.c -------------------------------------------------- */
void		print_symbol(FILE *out, t_ctx *c, t_symbol *symbol, uint64_t idx);
void		print_trace_line(FILE *out, t_ctx *c, t_parser_state *parse,
				const char *action);
void		print_tokens(FILE *out, t_ctx *c);
void		print_node_flags(FILE *out, uint8_t flags);
void		print_node_line(FILE *out, t_ctx *c, t_node *node, uint64_t idx);
void		print_stack(FILE *out, t_ctx *c, t_parser_state *parse);
void		print_nodes(FILE *out, t_ctx *c);

/* -------- debug_parse.c (parser trace helpers) --------------------------- */
void		build_rule_desc(char *buf, size_t size, int32_t action,
				t_ctx *c, t_rule *rule, t_parser_state *parse);
void		print_trace_step(t_ctx *c, t_parser_state *parse,
				const char *label);
void		log_rhs_symbols(t_ctx *c, t_parser_state *parse, t_rule *rule);
void		debug_trace_shift(t_ctx *c, t_parser_state *parse);
void		debug_parse_header(t_parser_state *parse);
void		debug_parse_arenas(t_ctx *c);
void		debug_parse_action(t_ctx *c, t_parser_state *parse,
				t_lalr_action action);

/* -------- debug_main.c ---------------------------------------------------- */
void		debug_print_read_line(t_ctx *c);
void		debug_print_after_parse(t_ctx *c, t_parser_state *parse);

/* -------- debug_utils.c -------------------------------------------------- */
void		print_char_info(unsigned char ch);
size_t		escape_into_buf(char *dst, size_t size, const char *src,
				size_t len);
void		print_escaped_str(FILE *out, const char *str);
void		print_escaped_strn(FILE *out, const char *str, size_t len);
void		parse_flag_list(const char *spec, uint8_t *mask,
				const char **names, const uint8_t *bits, uint8_t all);
void		parse_debug_args(int argc, char **argv, t_ctx *c);

#endif
