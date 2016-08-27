/* exec_pipeline.c - NutShell

 Copyright (c) 2016 Emanuel Valente <emanuelvalente@gmail.com>
              2016 Ariella Yamada  <ariella.y.b@gmail> and
              2016 Marcio Campos   <marciodscampos@gmail.com>
   Nutshell is derived from POSIXeg Fool Shell -
   https://gitlab.com/monaco/posixeg/shell/foolsh
   Copyright (c) 2015 Francisco Jose Monaco <monaco@icmc.usp.br>
   POSIXeg repository can be found at https://gitlab.com/monaco/posixeg
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



#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h>
#include <tparse.h>
#include <runcmd.h>
#include <native_commands.h>
#include <exec_pipeline.h>
#include <debug.h>


void exec_pipeline_one_command(pipeline_t *pipeline)
{
  int j, result;
  char *cmd;

  /*prepares cmd buffer*/
  cmd = (char*)malloc(sizeof(char) * MAX_BUFF_COMMAND);

  strncpy(cmd, pipeline->command[0][0], MAX_BUFF_COMMAND);


  /*process args*/
  for (j=1; pipeline->command[0][j]; j++)
    {
      strncat(cmd, " ", MAX_BUFF_COMMAND -1);
      strncat(cmd, pipeline->command[0][j], MAX_BUFF_COMMAND -1);
    }

      runcmd(cmd, &result, NULL);
      if(!IS_EXECOK(result))
        fprintf(stderr, "NutShell: %s: command not found\n", cmd);
}

void exec_pipeline_redir_input(pipeline_t *pipeline, int index)
{
  int j, result, tmp;
  char *cmd;
  int *io;

  io = (int*)malloc(sizeof(int) * 3);
  /*prepares cmd buffer*/
  cmd = (char*)malloc(sizeof(char) * MAX_BUFF_COMMAND);

  strncpy(cmd, pipeline->command[index][0], MAX_BUFF_COMMAND -1);


  /*process args*/
  for (j=1; pipeline->command[index][j]; j++)
    {
      strncat(cmd, " ", MAX_BUFF_COMMAND);
      strncat(cmd, pipeline->command[index][j], MAX_BUFF_COMMAND -1);
    }

  tmp = open(pipeline->file_in, O_RDONLY);
  fatal(tmp < 0, "Could not redirect input: file not found");
  io[0] = tmp;
  io[1] = STDOUT_FILENO;
  io[2] = STDERR_FILENO;

  runcmd(cmd, &result, io);

  if(!IS_EXECOK(result))
    fprintf(stderr, "NutShell: %s: command not found\n", cmd);

}


void exec_pipeline_redir_output(pipeline_t *pipeline, int index)
{
  int j, result, tmp;
  char *cmd;
  int *io;

  io = (int*)malloc(sizeof(int) * 3);

  /*prepares cmd buffer*/
  cmd = (char*)malloc(sizeof(char) * MAX_BUFF_COMMAND);

  strncpy(cmd, pipeline->command[index][0], MAX_BUFF_COMMAND -1);


  /*process args*/
  for (j=1; pipeline->command[index][j]; j++)
    {
      strncat(cmd, " ", MAX_BUFF_COMMAND);
      strncat(cmd, pipeline->command[index][j], MAX_BUFF_COMMAND -1);
    }

  tmp = open(pipeline->file_out, O_CREAT | O_TRUNC | O_RDWR , S_IRUSR | S_IWUSR);
  fatal(tmp < 0, "Could not redirect output");
  io[0] = STDIN_FILENO;
  io[1] = tmp;
  io[2] = STDERR_FILENO;

  runcmd(cmd, &result, io);

  if(!IS_EXECOK(result))
    fprintf(stderr, "NutShell: %s: command not found\n", cmd);

}

void exec_pipeline_redir_input_output(pipeline_t *pipeline, int index)
{
  int j, result, tmp1, tmp2;
  char *cmd;
  int *io;

  io = (int*)malloc(sizeof(int) * 3);

  /*prepares cmd buffer*/
  cmd = (char*)malloc(sizeof(char) * MAX_BUFF_COMMAND);

  strncpy(cmd, pipeline->command[index][0], MAX_BUFF_COMMAND);


  /*process args*/
  for (j=1; pipeline->command[index][j]; j++)
    {
      strncat(cmd, " ", MAX_BUFF_COMMAND);
      strncat(cmd, pipeline->command[index][j], MAX_BUFF_COMMAND -1);
    }

  tmp1 = open(pipeline->file_in, O_RDONLY);
  fatal(tmp1 < 0, "Could not redirect input");
  tmp2 = open(pipeline->file_out, O_CREAT | O_TRUNC | O_RDWR , S_IRUSR | S_IWUSR);
  fatal(tmp2 < 0, "Could not redirect output");

  io[0] = tmp1;
  io[1] = tmp2;
  io[2] = STDERR_FILENO;

  runcmd(cmd, &result, io);

  if(!IS_EXECOK(result))
    fprintf(stderr, "NutShell: %s: command not found\n", cmd);

}



int execute_pipeline(pipeline_t * pipeline, int pipeA[2], int index)
{
  int pid, pipeB[2], ret;

  if (index < 0)
    return -1;

  if (index > 0) {
      ret = pipe(pipeB);
      fatal(ret < 0, "Could not create pipe.");
    }


  pid = fork();
  fatal(pid < 0, "Could not create process");

  /* Parent: exec */
  if (pid > 0) {

      /* Not last, redirect input */
      if (index > 0) {
          close(0);
          ret = dup(pipeB[0]);
	  sysfail(ret <0, -1);
          close(pipeB[0]);
        }

      /* Not first, redirect output */
      if (index < pipeline->ncommands - 1) {
          close(1);
          ret = dup(pipeA[1]);
	  sysfail(ret <0, -1);
          close(pipeA[1]);
        }
      else

      /* Not used. */
      if (index > 0)
        close(pipeB[1]);

      /* Run command */
      execvp(pipeline->command[index][0], pipeline->command[index]);

      /* Child: recurse */
    } else {
      execute_pipeline(pipeline, pipeB, index - 1);
      return EXIT_SUCCESS;
    }

}

