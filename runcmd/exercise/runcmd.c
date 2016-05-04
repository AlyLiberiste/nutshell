/* runcmd.c - Execute a command as a subprocess. 

   Copyright (c) 2014, Francisco José Monaco <moanco@icmc.usp.br>

   This file is part of POSIXeg

   POSIXeg is free software: you can redistribute it and/or modify
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

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <string.h>

#include <runcmd.h>
#include <debug.h>


/* Executes 'command' in a subprocess. Information on the subprocess execution
   is stored in 'result' after its completion, and can be inspected with the
   aid of macros made available for this purpose. Argument 'io' is a pointer
   to an integer vector where the first, second and third positions store
   file descriptors to where standard input, output and error, respective, 
   shall be redirected; if NULL, no redirection is performed. On
   success, returns subprocess' pid; on error, returns 0. */

int runcmd (const char *command, int *result, int *io) /* ToDO: const char* */
{
  int pid, status, pipeid[2];
  int aux, i, tmp_result;
  char *args[RCMD_MAXARGS], *p, *cmd, buff;

  tmp_result = 0;

  /* Parse arguments to obtain an argv vector. */

  cmd = malloc ((strlen (command)+1) * sizeof(char));
  sysfail (!cmd, -1);
  p = strcpy (cmd, command);

  i=0;
  args[i++] = strtok (cmd, RCMD_DELIM);
  while ((i<RCMD_MAXARGS) && (args[i++] = strtok (NULL, RCMD_DELIM)));
  i--;

  /* Create a subprocess. */

  /*using pipe to discover if there was error of
   * execution: ex: ./foo (=wont set EXECOK)*/
  pipe(pipeid);

  pid = fork();
  sysfail (pid<0, -1);

  if (pid>0)			/* Caller process (parent). */
    {
      aux = wait (&status);
      sysfail (aux<0, -1);

      /*if aux has one byte -> EXECOK
       * otherwhise (has two bytes) -> ./foo
        won't set EXECOK*/
      aux = read(pipeid[0], &buff, 2);
      
      /* Collect termination mode. */
      if (WIFEXITED(status) && aux == 1)
        {
          tmp_result |= EXECOK;
          tmp_result |= NORMTERM;
          tmp_result |= (WEXITSTATUS(status) & RETSTATUS) ;
        }

      /*at this point (./fooo (no such file) !EXECOK
       * we just want to set exit returned value*/
      else
        {
         tmp_result |= (WEXITSTATUS(status) & RETSTATUS) ;

        }


    }
  else	/* Subprocess (child) */
    {

      /*write one byte on pipe*/
      buff = 0;
      write(pipeid[1], &buff, 1);
      aux = execvp (args[0], args);
      /*write more one byte (error) on pipe
       *at this point we have two bytes on it*/
      buff = 1;
      write(pipeid[1], &buff, 1);

      exit(EXECFAILSTATUS);
    }

  if (result && aux)
    *result = tmp_result;

  free (p);
  return pid;			/* Only parent reaches this point. */
}

/* If function runcmd is called in non-blocking mode, then function
   pointed by rcmd_onexit is asynchronously evoked upon the subprocess
   termination. If this variable points to NULL, no action is performed.
*/

void (*runcmd_onexit)(void) = NULL;

