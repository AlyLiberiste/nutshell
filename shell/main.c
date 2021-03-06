/* main.c - The NutShell

Copyright (c) 2016 Emanuel Valente <emanuelvalente@gmail.com>
              2016 Ariella Yamada  <ariella.y.b@gmail> and
              2016 Marcio Campos   <marciodscampos@gmail.com>

   Nutshell is derived from POSIXeg Fool Shell -
   https://gitlab.com/monaco/posixeg/shell/foolsh
   Copyright (c) 2015 Francisco Jose Monaco <monaco@icmc.usp.br>
   POSIXeg repository can be found at https://gitlab.com/monaco/posixeg

   This file is part of Nutshell
   Nutshell is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <tparse.h>
#include <debug.h>
#include <user_level.h>
#include <native_commands.h>
#include <exec_pipeline.h>
#include <runcmd.h>
#include <assert.h>

#define PROMPT "@NutShell:"

char *prompt;

/* void test(void); */

int go_on = 1;			/* This variable controls the main loop. */

void sigchld_handler (int sig) {
   int status;
     pid_t child;
    assert(sig == SIGCHLD);
    while ((child = waitpid(-1, &status, WNOHANG)) > 0) {
        /*...do something with exited child's status */
    }
}


void ignore_signals();


int main (int argc, char **argv)
{
  buffer_t *command_line;
  int i=0, aux;
  int result, pid, status;
  char *cmd;
  pipeline_t *pipeline;

  /*process the prompt string*/
  prompt = get_prompt();

  command_line = new_command_line ();

  pipeline = new_pipeline ();

  /*prepares cmd buffer*/
  cmd = (char*)malloc(sizeof(char) * MAX_BUFF_COMMAND);

  /* This is the main loop. */
  setpgid(0, 0);

  while (go_on)
    {
      /* Prompt. */
      ignore_signals();

      printf ("%s ", prompt);
      fflush (stdout);
      aux = read_command_line (command_line);
      sysfatal (aux<0);

      /* Parse command line (see tparse.*) */

      if (!parse_command_line (command_line, pipeline))
        {
          if(pipeline->ncommands < 1)
            continue;


	  if(RUN_FOREGROUND(pipeline))
	    {
	      /*just one command without redirect I/O*/
	      if(pipeline->ncommands == 1 && !REDIRECT_STDIN(pipeline)
		 && !REDIRECT_STDOUT(pipeline))
		{
		  /*check if the command is native from shell*/
		  result = parse_native_command(pipeline, 0);
		  if(result == EXIT_COMMAND)
		    {
		      go_on = 0;
		      continue;
		    }
		  else
		    {
		      if(result == REGULAR_NATIVE_COMMAND)
			continue;
		    }

		  exec_pipeline_one_command(pipeline);
		}

	      /*more than one command*/
	      /*possible to redirect*/
	      else {
		  if ( REDIRECT_STDIN(pipeline) &&  REDIRECT_STDOUT(pipeline))
		    exec_pipeline_redir_input_output(pipeline, i);

		  else if ( REDIRECT_STDOUT(pipeline))
		    exec_pipeline_redir_output(pipeline, i);

		  else

		    if ( REDIRECT_STDIN(pipeline))
		      exec_pipeline_redir_input(pipeline, i);

		    /*pipes */
		    else
		    {
		      pid = fork();
		      if(pid > 0)
			wait(&status);
		      else 
		      {
			execute_pipeline(pipeline, NULL, pipeline->ncommands -1);
			return EXIT_SUCCESS;
		      }
		    }
		}
	    }/*IF FOREGROUND*/

	  /*BACKGROUND*/
	  else
	    {
	      strncpy(cmd, pipeline->command[0][0], MAX_BUFF_COMMAND);
	      strncat(cmd, " &", MAX_BUFF_COMMAND);
	      runcmd(cmd, &result, NULL);

	    }
	}
    }

  release_command_line (command_line);
  release_pipeline (pipeline);
  free(prompt);
  free(cmd);
  printf("Bye for now!\n\n");

  return EXIT_SUCCESS;
}


void ignore_signals()
{
  signal(SIGTSTP, SIG_IGN);
  signal(SIGINT,  SIG_IGN);
  signal(SIGCHLD, sigchld_handler);
}


