#include "libft.h"
#include "structs.h"
#include "execute.h"
#include "envars.h"
#include "builtins.h"
#include "utils.h"
#include <errno.h>
#include <signal.h>
#include "error_handling.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void	exit_child(char *process_name)
{
	if (errno == 127)
	{
		ft_putstr_fd(process_name, 2);
		ft_putstr_fd(": Command not found\n", 2);
	}
	else if (errno != 0)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(process_name);
	}
	exit(errno);
}

int		pipe_setup(int fd[2], int flag)
{
	int	ret;

	ret = 0;
	if (flag == PIPE_OUT)
	{
		close(fd[0]);
		ret = dup2(fd[1], 1);
		if (ret < 0)
			return (errno);
		close(fd[1]);
	}
	if (flag == PIPE_IN)
	{
		close(fd[1]);
		ret = dup2(fd[0], 0);
		if (ret < 0)
			return (errno);
		close(fd[0]);
	}
	return (0);
}

int		fd_setup(t_token *token, t_cmd *cmd)
{
	int	ret;

	ret = 0;
	if (token->flag == OUT_FILE || token->flag == OUT_FILE_APPEND)
		ret = output_redirect(token);
	else if (token->flag == IN_FILE)
		ret = input_redirect(token);
	return (ret);
}

int		redir_setup(t_cmd *cmd)
{
	t_list	*current_token;
	int		ret;

	ret = 0;
	current_token = cmd->tokens;

	if (cmd->pipe_in[0])
		ret = pipe_setup(cmd->pipe_in, PIPE_IN);
	if (ret != 0)
		return (ret);
	while (current_token)
	{
		ret = fd_setup(current_token->content, cmd);
		if (ret != 0)
			return (ret);
		current_token = current_token->next;
	}
	if (cmd->pipe_out[0])
		ret = pipe_setup(cmd->pipe_out, PIPE_OUT);
	if (ret != 0)
		return (ret);
	return (ret);
}

void	child_process(t_cmd *cmd)
{
	int		ret;
	char	**custom_envp;

	errno = 0;
	ret = redir_setup(cmd);
	if (ret != 0)
		exit_child("");
	if (!*(cmd->params))
		exit_child("");
	if (cmd->builtin != MINI_BUILTIN_NONE)
	{
		errno = run_reserved(1, cmd);
		return (exit_child(*(cmd->params)));
	}
	custom_envp = get_envars_as_envp(cmd->mini);
	if (!custom_envp)
	{
		errno = ENOMEM;
		exit_child("");
	}
	if (!cmd->path)
		errno = 127;
	else
		execve(cmd->path, cmd->params, custom_envp);
	ft_free_double_ptr((void **)custom_envp);
	exit_child(*(cmd->params));
}
