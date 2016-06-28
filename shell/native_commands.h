/* native_commands.h - Header for the NutShell.

   Copyright (c) 2016 Emanuel Valente <emanuelvalente@gmail.com>

   Nutshell is derived from POSIXeg Fool Shell -
   https://gitlab.com/monaco/posixeg/shell/foolsh
   Copyright (c) 2014 Francisco Jose Monaco
   POSIXeg repository can be found at https://gitlab.com/monaco/posixeg

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef NATIVE_COMMANDS_H
#define NATIVE_COMMANDS_H

#define EXIT_COMMAND 2
#define REGULAR_NATIVE_COMMAND 1
#define USER_COMMAND 0


int parse_native_command(char*);

#endif /* NATIVE_COMMANDS_H*/

