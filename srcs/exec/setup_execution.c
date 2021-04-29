/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_execution.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahaas <bahaas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/13 14:06:30 by bahaas            #+#    #+#             */
/*   Updated: 2021/04/29 04:49:43 by bahaas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	close_fd(int fd)
{
	if (fd > 0)
		close(fd);
}

/*
   void	reset_fd(t_ms *ms)
   {
   dup2(ms->in, STDIN);
   dup2(ms->out, STDOUT);
   close_fd(ms->fdin);
   close_fd(ms->fdout);
   close_fd(ms->pipin);
   close_fd(ms->pipout);
   ms->fdin = -1;
   ms->fdout = -1;
   ms->pipin = -1;
   ms->pipout = -1;
   }

   t_cmd	*next_cmd_to_execute(t_cmd *cmd)
   {
   cmd = cmd->next;
   while (cmd && cmd->type_link != CMD_ARGS)
   {
   cmd = cmd->next;
   if (cmd && cmd->type_link == CMD_ARGS && cmd->prev->type_link != 5)
   cmd = cmd->next;
   if (cmd && (cmd->type_link == 6 ||
   cmd->type_link == 7 ||
   cmd->type_link == 8) && cmd->prev->type_link == 5)
   break ;
   }
   return (cmd);
   }
   */
/*
 ** 1. Execute cmd block per block (séparateur = ; )
 ** 2. After execution, we reset our fd to stdin & stdout for next command
 ** 3. On se déplace au prochain bloc de commande (premiere commande après un ;)
 */

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h> /* pid_t */
#include <unistd.h>

#define PROGNAME "pipeline-three-processes"

#define Close(FD) do {                                  \
	int Close_fd = (FD);                                \
	if (close(Close_fd) == -1) {                        \
		perror("close");                                  \
		fprintf(stderr, "%s:%d: close(" #FD ") %d\n",     \
				__FILE__, __LINE__, Close_fd);            \
	}                                                   \
}while(0)

static void
report_error_and_exit_helper(const char* message, void (*exit_func)(int))
{
	fprintf(stderr, "%s: error: %s (system: %s)\n", PROGNAME, message ? message : "", strerror(errno));
	exit_func(EXIT_FAILURE);
}

static void report_error_and_exit(const char* message)
{
	report_error_and_exit_helper(message, exit);
}

static void _report_error_and_exit(const char* message)
{
	report_error_and_exit_helper(message, _exit);
}


/* move oldfd to newfd */
static void redirect(int oldfd, int newfd)
{
	if (oldfd != newfd) {
		if (dup2(oldfd, newfd) != -1)
			Close(oldfd); /* successfully redirected */
		else
			_report_error_and_exit("dup2");
	}
}

static void run(char* argv[], int in, int out, t_ms *ms, t_cmd *cmd, int i)
{
	redirect(in, STDIN_FILENO);   /* <&in  : child reads from in */
	redirect(out, STDOUT_FILENO); /* >&out : child writes to out */
	execvp(argv[0], argv);
	_report_error_and_exit("execp");
	//launch_exec(ms, cmd);
	//_report_error_and_exit("execve");
}

void	setup_execution(t_ms *ms, t_cmd *cmd)
{
	int n_command_to_exec;
	t_cmd *tmp;

	n_command_to_exec = 0;
	tmp = cmd;
	while(tmp)
	{
		n_command_to_exec++;
		tmp = tmp->next;
	}
	printf("n_command_to_exec : %d\n", n_command_to_exec);
	printf("Minishell pid : %d\n", getpid());
	/* run all commands but the last */
	int i = 0, in = STDIN_FILENO; /* the first command reads from stdin */
	while(cmd && cmd->next)
	{
		int fd[2]; /* in/out pipe ends */
		pid_t pid; /* child's pid */

		if (pipe(fd) == -1)
			report_error_and_exit("pipe");
		else if ((pid = fork()) == -1)
			report_error_and_exit("fork");
		else if (pid == 0) 
		{ /* run command[i] in the child process */
			Close(fd[0]); /* close unused read end of the pipe */
			//printf("execute cmd : %s with pid : %d\n", (char *)command[i][0], getpid());
			//run((char * const*)command[i], in, fd[1], ms, cmd); /* $ command < in > fd[1] */
			printf("execute cmd : %s with pid : %d\n", cmd->content[0], getpid());
			run(cmd->content, in, fd[1], ms, cmd, 0); /* $ command < in > fd[1] */
		}
		else
		{ /* parent */
			assert (pid > 0); //doit check ce que c'est
			Close(fd[1]); /* close unused write end of the pipe */
			Close(in);    /* close unused read end of the previous pipe */
			in = fd[0]; /* the next command reads from here */
		}
		i++;
		cmd = cmd->next;
	}
	//lance last command pr avoir la bonne valeur de last_ret, les child ne passent pas ici
	//car ils s'exec avant et s'exit donc ne passe pas par ici.
	//printf("execute last cmd : %s with pid : %d\n", (char *)command[i][0], getpid());
	//run((char * const*)command[i], in, STDOUT_FILENO, ms, cmd); /* $ command < in */
	printf("execute last cmd : %s with pid : %d\n", cmd->content[0], getpid());
	run(cmd->content, in, STDOUT_FILENO, ms, cmd, 1); /* $ command < in */
}

