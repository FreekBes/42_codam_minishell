#include "structs.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

//return 0 on no error, -2 on ENOMEM, -1 on command found

int		find_abs_path(t_cmd *cmd, t_mini *mini)
{
	char	*temp;
	int		i;

	i = 0;
	while (mini->paths[i])
	{
		cmd->path = ft_pathjoin(mini->paths[i], *(cmd->params));
		if (!cmd->path)
			return (-2);
		if (access(cmd->path, R_OK) == 0)
			return (0);
		free(cmd->path);
		i++;
	}
	return (-1);
}

int		check_command(t_cmd *cmd, t_mini *mini)
{
	int		ret;

	if (!*(cmd->params))
		return (0);
	if (ft_strchr(*(cmd->params), '/'))
	{
		if (access(*(cmd->params), R_OK) == 0)
		{
			cmd->path = ft_strdup(*(cmd->params));
			return (0);
		}
		return (-1);
	}
	ret = find_abs_path(cmd, mini);
	if (ret == -1)
		cmd->path = NULL;
	return (ret);
}
