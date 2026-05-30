/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nribakov <nribakov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/22 21:48:29 by sancuta           #+#    #+#             */
/*   Updated: 2026/05/30 17:23:02 by nribakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef DEBUG_H
# define DEBUG_H

# include "arena.h"

typedef enum e_arena_type
{
	AT_NONE,	// TODO: should i init arenas as NONE, with everything zeroed out, and then assign arena to the correct type?
	AT_STRING,
	AT_PROMPT,
	AT_TOKEN,
	AT_STACK,
	AT_CMD,
	AT_COUNT,
} t_arena_type;

typedef struct s_env
{
	char **envp;
}	t_env;

typedef struct s_ctx
{
	t_arena	arena[AT_COUNT];
	char  	*read_line;
	t_env	env;
}	t_ctx;

void	print_arena(t_arena *arena);
void	print_token(t_ctx *c, size_t token_idx);
void	poison_sentinel(t_arena *arena);
void	print_arena(t_arena *arena);

#endif
