dnl
dnl Copyright (c) Cisco Systems Inc., 2009
dnl
dnl This program is free software;  you can redistribute it and/or modify
dnl it under the terms of the GNU General Public License as published by
dnl the Free Software Foundation; either version 2 of the License, or
dnl (at your option) any later version.
dnl
dnl This program is distributed in the hope that it will be useful,
dnl but WITHOUT ANY WARRANTY;  without even the implied warranty of
dnl MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
dnl the GNU General Public License for more details.
dnl
dnl You should have received a copy of the GNU General Public License
dnl along with this program;  if not, write to the Free Software
dnl Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
dnl
dnl Author: Garrett Cooper <yanegomi@gmail.com>
dnl

dnl
dnl LTP_CHECK_SYSCALL_NUMA
dnl ----------------------------
dnl
AC_DEFUN([LTP_CHECK_SYSCALL_NUMA],
[dnl
AC_CHECK_HEADERS([numa.h numaif.h],[
	LTP_SYSCALL_NUMA_HEADERS=yes
	AC_CHECK_FUNCS(numa_alloc_onnode,numa_move_pages)
]
	AC_CHECK_LIB(numa,numa_available,[
NUMA_CPPFLAGS="-DNUMA_VERSION1_COMPATIBILITY"
NUMA_LIBS="-lnuma"
	])
AC_SUBST(NUMA_CPPFLAGS)
AC_SUBST(NUMA_LIBS)
)])
