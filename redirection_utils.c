/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   redirection_utils.c                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: jgalloni <jgalloni@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/05 19:59:59 by jgalloni      #+#    #+#                 */
/*   Updated: 2021/12/06 21:20:16 by jgalloni      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <unistd.h>
#include "utils.h"
#include<sys/wait.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>

int	input_redirect(int mode, t_cmd *cmd)
{
	int	fd;

	if (mode & IN_FILE)
		fd = open(cmd->in_file, O_RDONLY);
	else
		fd = cmd->in_fd;
	if (fd == -1)
		return (-1);
	dup2(fd, 0);
	close(fd);
	return (1);
}

int	output_redirect(int mode, t_cmd *cmd)
{
	int	fd;

	if (mode & OUT_FILE)
		fd = open(cmd->out_file, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	else
		fd = cmd->out_fd;
	if (fd == -1)
		return (-1);
	dup2(fd, 1);
	close(fd);
	return (1);
}

int	cmd_redirection(t_cmd *cmd, t_list **current_token)
{
	t_token *token;
	int	ret;

	token = (t_token *)((*current_token)->content);
	if (token->flag == OUTPUT_REDIR)
	{
		cmd->out_file = token->content;
		cmd->mode = cmd->mode | OUT_FILE;
		ret = output_redirect(cmd->mode, cmd);
	}
	else if (token->flag == INPUT_REDIR)
	{
		cmd->in_file = token->content;
		cmd->mode = cmd->mode | IN_FILE;
		ret = input_redirect(cmd->mode, cmd);
	}
	else if (token->flag == PIPE_OUT)
	{
		cmd->out_fd = ((int*)(token->content))[1];
		cmd->mode = OUT_FD;
		ret = output_redirect(cmd->mode, cmd);
	}
	else if (token->flag == PIPE_IN)
	{
		cmd->in_fd = ((int*)(token->content))[0];
		cmd->mode = IN_FD;
		ret = input_redirect(cmd->mode, cmd);
	}
	return (0);
}
