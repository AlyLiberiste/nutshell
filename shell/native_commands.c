/* native_commands.c - check if the command is a shell native command.
 * If so, it executes the command.

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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

#include <native_commands.h>
char *native_commands[] = {"exit", "jobs", "cd", "pwd"};

int parse_native_command(pipeline_t *pipeline, int index)
{
  char *cmd;
  int i=0, j=0;

  /*prepares cmd buffer*/
  cmd = (char*)malloc(sizeof(char) * MAX_BUFF_COMMAND);

  strncpy(cmd, pipeline->command[index][0], MAX_BUFF_COMMAND);


  /*process args*/
  for (j=1; pipeline->command[index][j]; j++)
    {
      strncat(cmd, " ", MAX_BUFF_COMMAND);
      strncat(cmd, pipeline->command[index][j], MAX_BUFF_COMMAND);
    }

  while(native_commands[i])
    {
      if(strncmp(native_commands[i], cmd, strlen(native_commands[i])) == 0)
        {
          if(strcmp(cmd, "exit") == 0)
            {
              free(cmd);
              return EXIT_COMMAND;
            }
          else
            {
              if(strncmp(cmd, "jobs", 4) == 0)
                /*pass first argument (jobid)*/
                exec_native_jobs(pipeline->command[index][1]);

              if(strncmp(cmd, "cd", 2) == 0)
                /*pass first argument (dir)*/
                exec_native_cd(pipeline->command[index][1]);

              if(strncmp(cmd, "pwd", 3) == 0)
                exec_native_pwd();

            }

          free(cmd);
          return REGULAR_NATIVE_COMMAND;
        }
      i++;

    }

  free(cmd);
  return USER_COMMAND;

}

void exec_native_jobs(char *arg)
{
  printf("====> jobs %s\n", arg);
}

void exec_native_cd(char *arg)
{
  int ret;
  char *homedir;

  /*user want to redir to home*/
  if(!arg)
    {
      if ((homedir = getenv("HOME")) == NULL)
        homedir = getpwuid(getuid())->pw_dir;

      ret = chdir(homedir);
      if(ret < 0)
        fprintf(stderr, "Could not change to directory: %s\n", homedir);
    }
  else
    {
      ret = chdir(arg);

      if(ret < 0)
        fprintf(stderr, "Could not change to directory: %s\n", arg);
    }


}

void exec_native_pwd()
{
  char *ret;
  char buff[512];

  ret = getcwd(buff, sizeof(buff));
  if(!ret)
    fprintf(stderr, "Current directory is too long!\n");
  else
    /*prints current directory*/
      printf("%s\n", buff);
}


