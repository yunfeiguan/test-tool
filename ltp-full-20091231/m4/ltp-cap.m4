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
dnl LTP_CHECK_CAPABILITY_SUPPORT
dnl ----------------------------
dnl
AC_DEFUN([LTP_CHECK_CAPABILITY_SUPPORT],[
AC_CHECK_HEADERS(sys/capability.h,[
	AC_CHECK_HEADERS(attr/xattr.h)
	LTP_CAPABILITY_SUPPORT=yes
	AC_CHECK_LIB(cap,cap_free,[
		AC_CHECK_LIB(cap,cap_from_text,[
			AC_CHECK_LIB(cap,cap_set_proc,[
				AC_CHECK_LIB(cap,cap_compare,[
					CAP_LIBS="-lcap"
				])
			])
		])
	])
	AC_CHECK_DECLS([CAP_BSET_DROP, CAP_BSET_READ, PR_CAPBSET_READ, cap_compare, cap_free, cap_from_text, cap_get_proc, cap_set_file, cap_set_flag, cap_set_proc, cap_to_text],[],[],[dnl
#include <sys/capability.h>
]) dnl AC_CHECK_DECLS
])]
AC_SUBST(CAP_LIBS)
)
