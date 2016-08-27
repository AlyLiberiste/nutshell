# NutShell

OVERVIEW
--------------------------------------------------
This is a implementation of a shell for the discipline of [Operating
System II][SOII]. It was taken in the course of Computer Science (ICMC - USP).

DISTRIBUTION AND USE
--------------------------------------------------
Users can find a distribution in the tab [releases][release].

Run NutShell with:

```bash
  $ cd nutshell
  $ cd shell
  $ ./nutshell
```

NutShell can execute external commands like `ls` and `clear` and the following internal commands:

* `cd`
* `exit`


NutShell will also redirect input and output (NutShell's parser will handle the right order and whitespaces) and use pipe to run multiple programs at the same time.



BUILD AND INSTALL
--------------------------------------------------
For developers and builders follow the steps below.

Clone NutShell repository:

```bash
  $ git clone https://github.com/SOII-2016/nutshell.git
```

NutShell development environment rely on the [GNU Build system
(Autotools)][autotools].  Developers may be able to bootstrap the project by issuing:

```bash
  $ cd nutshell
  $ ./autogen.sh
```

The provided autogen.sh script should execute autoconf, automake, libtool
and related programs in the right order.

Builders should be able to run:

```bash
 $ ./configure --prefix=<somepath>
 $ make
 $ make install
```

Builders should also be able to create distribution archives:

```bash
 $ make dist
```

The distributed files should contain all the files needed for building NutShell at the builder's environment, without the need of Autotools.

LICENSE
--------------------------------------------------
[GPLv3][license]

NutShell, Copyright (c) 2016 Luan G. O. <luan.orlandi@usp.br>,
Renato J. A. <renato.jose93@gmail.com> and Wesley T. <wes@usp.br>
NutShell is derived from [POSIX-eg][credits], Copyright (c) 2015 Monaco F. J. <<monaco@icmc.usp.br>>

This file is part of NutShell.

NutShell is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

NutShell is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with NutShell.  If not, see <http://www.gnu.org/licenses/>.

[SO2]: <https://github.com/SOII-2016>
[release]: <https://github.com/SOII-2016/nutshell/releases>
[autotools]: <http://www.gnu.org/software/automake/manual/html_node/Autotools-Introduction.html>
[license]: <COPYING>
[credits]: <https://gitlab.com/monaco/posixeg>
