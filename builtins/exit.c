/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exit.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: fbes <fbes@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/19 22:19:30 by fbes          #+#    #+#                 */
/*   Updated: 2022/01/23 18:00:40 by jgalloni      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "../utils.h"

int	mini_exit(t_cmd *cmd)
{
	printf("exit\n");
	if (char_array_len(cmd->params) > 1)
	{
		printf("minishell: exit codes are not implemented (not required)\n");
		// maybe it is after all? "no arguments" is not stated in the subject
		// but it is stated for env, so maybe arguments != options in subject
	}
	exit_shell_w_error(cmd, -2);
	return (0);
}
