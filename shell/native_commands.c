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

#include <native_commands.h>
#include <string.h>

char *native_commands[] = {"exit", "jobs"};

int parse_native_command(char *cmd)
{
  int i=0;

  while(native_commands[i])
    {
      if(strcmp(native_commands[i++], cmd) == 0)
        {
          if(strcmp(cmd, "exit") == 0)
            return EXIT_COMMAND;
          else
            return REGULAR_NATIVE_COMMAND;
        }

    }

  return USER_COMMAND;

}
