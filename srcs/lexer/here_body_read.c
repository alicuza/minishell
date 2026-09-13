/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_body_read.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sancuta <sancuta@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 19:46:33 by sancuta           #+#    #+#             */
/*   Updated: 2026/09/13 19:46:47 by sancuta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int32_t	create_here_temp(int32_t *wfd)
{
	char	*name;
	char	*pid;
	int32_t	rfd;

	*wfd = -1;
	pid = ft_itoa(getpid());
	if (!pid)
		return (-1);
	name = ft_strjoin(HEREDOC_TMP, pid);
	free(pid);
	if (!name)
		return (-1);
	*wfd = open(name, O_CREAT | O_EXCL | O_RDWR | O_TRUNC, 0600);
	if (*wfd == -1 && errno == EEXIST)
	{
		unlink(name);
		*wfd = open(name, O_CREAT | O_EXCL | O_RDWR | O_TRUNC, 0600);
	}
	rfd = -1;
	if (*wfd != -1)
		rfd = open(name, O_RDONLY);
	unlink(name);
	free(name);
	return (rfd);
}

/* final fd consumed/closed by exec wiring */
void	get_here_doc(t_ctx *c, t_lexer_state *lex)
{
	t_node		*node;
	char		*here_end;
	int32_t		wfd;
	int32_t		rfd;

	node = get_node_from_idx(c, get_symbol_from_top(c, 1)->node_idx);
	here_end = get_ptr_from_offset(&c->arena[AT_STRING],
			node->data.redir.arena_offset);
	rfd = create_here_temp(&wfd);
	if (wfd == -1 || rfd == -1)
		msh_error("heredoc", "temporary file", strerror(errno));
	while ((c->read_line[lex->char_idx]
			|| read_here_line(c, lex, here_end))
		&& !here_line_ends(c, lex, here_end))
	{
		if (wfd != -1)
			write_here_line(c, wfd, lex, node);
	}
	ft_close_fd(&wfd);
	if (lex->flags & LEX_INTERRUPTED)
		ft_close_fd(&rfd);
	node->data.redir.fd = rfd;
}
