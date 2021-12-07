/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exit.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jgalloni <jgalloni@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/11/26 23:45:59 by jgalloni      #+#    #+#                 */
/*   Updated: 2021/12/07 19:46:43 by fbes          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "readline/readline.h"
#include "custom_errors.h"

/**
  * @brief Handle signals
  * SIGINT is ctrl c
  * -1 is passed in case of EOF
  */

void	sig_handler(int sig)
{
	if (sig == 137)
	{
		write(2, "\nOut of memory\n", 15);
		exit(ENOMEM);
	}
	else if (sig == -1)
	{
		printf("Goodbye\n");
		exit(0);
	}
	else if (sig == SIGINT)
	{
		printf("\n"); // Move to a new line
		rl_replace_line("", 0); // Clear the previous text
		printf("\x1b[1A\033[11C\x1b[0K\n");
		// Hooray for ANSI Escape Sequences!
		// \x1b stands for the ESCAPE key, then [1A goes up one line ([2A would go up two lines)
		// then [12C moves the cursor 12 places to the right
		// [0K removes all character from that position til the end of the line in the terminal
		rl_on_new_line(); // Regenerate the prompt on a newline
		rl_redisplay();
	}
}

/**
  * @brief Prints a custom or errno message and exits the process
  * @param err An error code. If set to -2, nothing is printed.
  */

void	exit_shell_w_error(int err)
{
	if (err == -1)
	{
		printf("\x1b[1A\033[11Cexit\n");
		rl_replace_line("exit", 1);
		exit(errno);
	}
	else if (err == -2)
	{
		exit(errno);
	}
	else if (err == CMDNF)
	{
		printf("minishell: command not found\n");
		exit (CMDNF);
	}
	else
	{
		perror("minishell");
		exit(errno);
	}
}
