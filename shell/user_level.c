/* user_level.c - get the user data information for NutShell

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
#include <unistd.h>
#include <string.h>

char *get_prompt()
{
  char *login;
  char *prompt;
  unsigned char prompt_total_bytes;

  /*get login name*/
  login = getlogin();

  /*@NutShell]$: (13 chars) + '[' login size*/
  prompt_total_bytes = (sizeof(char) * 13) + (sizeof(char) * (strlen(login) + 2) );
  prompt = (char*)malloc(prompt_total_bytes);

  /*concats login@NutShell:*/
  snprintf(prompt, prompt_total_bytes, "[%s%s]$", login, "@NutShell");

  return prompt;

}
