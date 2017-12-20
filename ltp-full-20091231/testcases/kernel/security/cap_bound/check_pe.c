/******************************************************************************/
/*                                                                            */
/* Copyright (c) International Business Machines  Corp., 2007, 2008           */
/*                                                                            */
/* This program is free software;  you can redistribute it and/or modify      */
/* it under the terms of the GNU General Public License as published by       */
/* the Free Software Foundation; either version 2 of the License, or          */
/* (at your option) any later version.                                        */
/*                                                                            */
/* This program is distributed in the hope that it will be useful,            */
/* but WITHOUT ANY WARRANTY;  without even the implied warranty of            */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See                  */
/* the GNU General Public License for more details.                           */
/*                                                                            */
/* You should have received a copy of the GNU General Public License          */
/* along with this program;  if not, write to the Free Software               */
/* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA    */
/*                                                                            */
/******************************************************************************/
/*
 * File: check_pe.c
 * Author: Serge Hallyn
 * check whether CAP_SYS_ADMIN is in pE.
 * return PASS if
 *	*  argv[0] is 1 and CAP_SYS_ADMIN is in PE, or
 * 	* argv[0] is 0 and CAP_SYS_ADMIN is not in pE
 * otherwise return FAIL
 */

#include <errno.h>
#include "config.h"
#if HAVE_SYS_CAPABILITY_H
#include <sys/capability.h>
#endif
#include <sys/prctl.h>
#include <test.h>

char *TCID = "check_pe";
int TST_TOTAL=1;

int errno;

int main(int argc, char *argv[])
{
#if HAVE_SYS_CAPABILITY_H
	int ret = 1;
	cap_flag_value_t f;
	cap_t cur;
	int n;

	if (argc != 2) {
		tst_resm(TBROK, "Usage: check_pe [0|1]\n");
		tst_exit();
	}
	n = atoi(argv[1]);
	if (n != 0 && n != 1) {
		tst_resm(TBROK, "Usage: check_pe [0|1]\n");
		tst_exit();
	}

#if HAVE_DECL_CAP_GET_FLAG
#if HAVE_DECL_CAP_GET_PROC
	cur = cap_get_proc();
	ret = cap_get_flag(cur, CAP_SYS_ADMIN, CAP_EFFECTIVE, &f);
#else
	ret = -1;
	errno = ENOSYS;
#endif
#else
	ret = -1;
	errno = ENOSYS;
#endif
	if (ret) {
		tst_resm(TBROK, "cap_get_flag failed (errno %d)\n", errno);
		tst_exit();
	}

#if HAVE_DECL_CAP_FREE
	cap_free(cur);
#endif
	if (n == 1) {
		if (f == CAP_SET) {
			tst_resm(TPASS, "cap is in pE\n");
			tst_exit();
		}
		tst_resm(TFAIL, "Cap is not in pE\n");
		tst_exit();
	}
	if (f == CAP_CLEAR) {
		tst_resm(TPASS, "cap is not in pE\n");
		tst_exit();
	}
	tst_resm(TFAIL, "Cap is in pE\n");
#else
	tst_resm(TCONF, "System doesn't have POSIX capabilities.");
#endif
	tst_exit();
}
