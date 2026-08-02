#include "minishell.h"

t_symbol_type	classify_token(t_ctx *c, t_token *token)
{
	char	*token_body;

	token_body = get_ptr_from_offset(&c->arena[AT_STRING], token->offset);
	if (!ft_strncmp(token_body, NL, ft_strlen(NL)))
		return (SYM_NEWLINE);
	else if (!ft_strncmp(token_body, DLESS, ft_strlen(DLESS) + 1))
		return (SYM_DLESS);
	else if (!ft_strncmp(token_body, DGREAT, ft_strlen(DGREAT) + 1))
		return (SYM_DGREAT);
	else if (!ft_strncmp(token_body, AND_IF, ft_strlen(AND_IF) + 1))
		return (SYM_AND_IF);
	else if (!ft_strncmp(token_body, OR_IF, ft_strlen(OR_IF) + 1))
		return (SYM_OR_IF);
	else if (!ft_strncmp(token_body, PIPE, ft_strlen(PIPE) + 1))
		return (SYM_PIPE);
	else if (!ft_strncmp(token_body, LESS, ft_strlen(LESS) + 1))
		return (SYM_LESS);
	else if (!ft_strncmp(token_body, GREAT, ft_strlen(GREAT) + 1))
		return (SYM_GREAT);
	else if (!ft_strncmp(token_body, OPAR, ft_strlen(OPAR) + 1))
		return (SYM_OPAR);
	else if (!ft_strncmp(token_body, CPAR, ft_strlen(CPAR) + 1))
		return (SYM_CPAR);
	return (SYM_WORD);
}
