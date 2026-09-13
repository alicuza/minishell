/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_pathname.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nribakov <nribakov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 20:58:41 by sancuta           #+#    #+#             */
/*   Updated: 2026/09/14 00:30:00 by sancuta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "minishell.h"

char	*add_prefix(char *path, const char *pathname)
{
	char	*tmp;
	char	*tmp1;

	tmp = NULL;
	tmp1 = NULL;
	tmp = ft_strjoin(path, "/");
	if (tmp != NULL)
	{
		tmp1 = ft_strjoin(tmp, pathname);
		free(tmp);
	}
	return (tmp1);
}

static int	handle_executable(t_command_ctx *cmd_ctx, char *tmp,
		char *first_found)
{
	free(cmd_ctx->pathname);
	free(first_found);
	cmd_ctx->pathname = tmp;
	return (EXIT_SUCCESS);
}

static int	check_path(t_command_ctx *cmd_ctx, char *path, char **first_found)
{
	struct stat	st;
	char		*tmp;

	if (path[0] == '\0')
		tmp = ft_strjoin("./", cmd_ctx->pathname);
	else
		tmp = add_prefix(path, cmd_ctx->pathname);
	if (!tmp)
		return (EXIT_FAILURE);
	if (access(tmp, F_OK) || stat(tmp, &st) || !S_ISREG(st.st_mode))
	{
		free(tmp);
		return (EXIT_SUCCESS);
	}
	if (access(tmp, X_OK) == 0)
		return (handle_executable(cmd_ctx, tmp, *first_found));
	else if (*first_found == NULL)
		*first_found = tmp;
	if (*first_found != tmp)
		free(tmp);
	return (EXIT_SUCCESS);
}

static int	search_in_paths(char **paths, t_command_ctx *cmd_ctx)
{
	char		*first_found;
	int			i;

	first_found = NULL;
	i = -1;
	while (paths[++i] != NULL)
	{
		if (check_path(cmd_ctx, paths[i], &first_found) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	free(cmd_ctx->pathname);
	cmd_ctx->pathname = first_found;
	return (EXIT_SUCCESS);
}

int	get_pathname(t_ctx *c, t_command_ctx *cmd_ctx)
{
	char	*path;
	char	**paths;
	int		status;

	path = env_get(&c->env, PATH);
	if (is_empty_str(path))
	{
		status = get_pathname_from_current_dir(cmd_ctx);
		free(path);
		return (status);
	}
	else
	{
		paths = ft_split_with_empty(path, ':');
		if (!paths)
			return (free(path), EXIT_FAILURE);
		status = search_in_paths(paths, cmd_ctx);
		free_str_arr(paths);
		return (free(path), status);
	}
}
