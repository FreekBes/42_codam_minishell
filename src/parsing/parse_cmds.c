#include "structs.h"
#include "debug.h"
#include "utils.h"
#include "builtins.h"
#include "custom_errors.h"
#include <stdlib.h>
#include "parse.h"
#include <stdio.h>
#include "envars.h"

int	parse_operator(char *prompt, int index, t_cmd *cmd)
{
	int	ret;

	ret = 0;
	if (prompt[index] == '<' || prompt[index] == '>' ||
		ft_isdigit(prompt[index]))
		ret = parse_input_redir(prompt + index, cmd);
	return(ret);
}

int	parse_cmd(t_list *cmd, char *prompt)
{
	int	ret;
	int	index;
	t_cmd	*current_cmd;

	ret = 0;
	index = 0;
	current_cmd = (t_cmd *)(cmd->content);
	current_cmd->params = ft_calloc(1, sizeof(char *));
	if (!(current_cmd->params))
		return (ENOMEM);
	while (prompt[index])
	{
		ret = parse_params(prompt + index, &(current_cmd->params));
		if (ret < 0)
			return (ret);
		index += ret;
		if (!prompt[index])
			break;
		ret = parse_operator(prompt, index, current_cmd);
		if (ret < 0)
			return (ret); // ENOMEM OR PARSE ERROR
		index += ret;
	}
	return (0);
}

t_list	*new_cmd(t_mini *mini)
{
	t_cmd	*cmd;
	t_list	*cmd_instance;

	cmd = (t_cmd *)ft_calloc(1, sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->mini = mini;
	cmd->builtin = MINI_BUILTIN_NONE;
	cmd_instance = ft_lstnew(cmd);
	if (!cmd_instance)
	{
		free(cmd);
		return (NULL);
	}
	ft_lstadd_back(&(mini->cmds), cmd_instance);
	return (cmd_instance);
}

int	expand_params(t_cmd *cmd)
{
	int	i;
	int	ret;

	i = 0;
	while ((cmd->params)[i])
	{
		ret = parse_envars(cmd->mini->envars, &((cmd->params)[i]));
		if (ret != 0)
			return (ENOMEM);
		i++;
	}
	return (0);
}

int	setup_cmds(t_mini *mini, char **prompts)
{
	int	i;
	t_list	*current_cmd;
	int ret;

	ret = 0;
	i = 0;
	while (prompts[i])
	{
		current_cmd = new_cmd(mini);
		if (current_cmd == NULL)
				return(ENOMEM);
		if (i > 0)
			((t_cmd *)(current_cmd->content))->pipe_in[0] = 1;
		ret = parse_cmd(current_cmd, prompts[i]);
		if (ret != 0)
			return (ret);
		if (prompts[i + 1])
			((t_cmd *)(current_cmd->content))->pipe_out[0] = 1;
		i++;
	}
	ret = expand_params(current_cmd->content);
	return (ret);
}
