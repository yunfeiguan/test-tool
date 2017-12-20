/*
 *
 *   Copyright (c) International Business Machines  Corp., 2001
 *
 *   This program is free software;  you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY;  without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
 *   the GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program;  if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

/*
 * NAME
 *	setrlimit01.c
 *
 * DESCRIPTION
 *	Testcase to check the basic functionality of the setrlimit system call.
 *
 * ALGORITHM
 *	Use the different commands like RLIMIT_NOFILE, RLIMIT_CORE,
 *	RLIMIT_FSIZE, and, RLIMIT_NOFILE, and test for different test
 *	conditions.
 *
 * USAGE:  <for command-line>
 *  msgctl01 [-c n] [-f] [-i n] [-I x] [-P x] [-t]
 *     where,  -c n : Run n copies concurrently.
 *             -f   : Turn off functionality Testing.
 *             -i n : Execute test n times.
 *             -I x : Execute test for x seconds.
 *             -P x : Pause for x seconds between iterations.
 *             -t   : Turn on syscall timing.
 *
 * HISTORY
 *	07/2001 Ported by Wayne Boyer
 *
 * RESTRICTIONS
 *	Must run test as root.
 */
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <wait.h>
#include <stdlib.h>
#include <malloc.h>
#include "test.h"
#include "usctest.h"

char *TCID = "setrlimit01";
int TST_TOTAL = 1;
extern int Tst_count;

void setup(void);
void cleanup(void);
void test1(void);
void test2(void);
void test3(void);
void test4(void);
void sighandler(int);

char filename[40] = "";
struct rlimit save_rlim, rlim, rlim1;
int nofiles, fd, bytes, i, status, fail = 0;
char *buf = "abcdefghijklmnopqrstuvwxyz";
pid_t pid;

int main(int ac, char **av)
{
	int lc;			/* loop counter */
	char *msg;		/* message returned from parse_opts */

	/* parse standard options */
	if ((msg = parse_opts(ac, av, (option_t *) NULL, NULL)) != (char *)NULL) {
		tst_brkm(TBROK, tst_exit, "OPTION PARSING ERROR - %s", msg);
	 /*NOTREACHED*/}

	setup();		/* set "tstdir", and "fname" vars */

	/* check looping state if -i option given */
	for (lc = 0; TEST_LOOPING(lc); lc++) {

		/* reset Tst_count in case we are looping */
		Tst_count = 0;

		test1();
		test2();
		test3();
		/* reset saved conditions */
		if ((setrlimit(RLIMIT_NPROC, &save_rlim)) == -1) {
			tst_brkm(TBROK, cleanup, "setrlimit failed to reset "
				 "RLIMIT_NPROC, errno = %d", errno);
		}
		test4();
	}
	cleanup();

	 /*NOTREACHED*/ return 0;

}

/*
 * test1 - Test for RLIMIT_NOFILE
 */
void test1()
{
	rlim.rlim_cur = 100;
	rlim.rlim_max = 100;

	TEST(setrlimit(RLIMIT_NOFILE, &rlim));

	if (TEST_RETURN == -1) {
		tst_resm(TFAIL, "setrlimit failed to set "
			 "RLIMIT_NOFILE, errno = %d", errno);
		return;
	}

	if (!STD_FUNCTIONAL_TEST) {
		tst_resm(TPASS, "call succeeded");
		return;
	}

	nofiles = getdtablesize();

	if (nofiles != 100) {
		tst_resm(TFAIL, "setrlimit failed, expected "
			 "100, got %d", nofiles);
		return;
	}

	tst_resm(TPASS, "RLIMIT_NOFILE functionality is correct");
}

/*
 * test2 - Test for RLIMIT_FSIZE
 */
void test2()
{
	/*
	 * Since we would be altering the filesize in the child,
	 * we need to "sync", ie. fflush the parent's write buffers
	 * here.  This is because the child will inherit the parent's
	 * write buffer, and while exitting it would try to fflush it.
	 * Since its filesize is truncated to only 10 bytes, the
	 * fflush attempt would fail, and the child would exit with
	 * an wired value!  So, it is essential to fflush the parent's
	 * write buffer HERE
	 */
	fflush(stdout);

	/*
	 * Spawn a child process, and reduce the filesize to
	 * 10 by calling setrlimit(). We can't do this in the
	 * parent, because the parent needs a bigger filesize as its
	 * output will be saved to the logfile (instead of stdout)
	 * when the testcase (parent) is run from the driver.
	 */
	if ((pid = FORK_OR_VFORK()) == -1) {
		tst_brkm(TBROK, cleanup, "fork() failed");
	}

	if (pid == 0) {
		rlim.rlim_cur = 10;
		rlim.rlim_max = 10;
		if ((setrlimit(RLIMIT_FSIZE, &rlim)) == -1) {
			exit(1);
		}

		if ((fd = creat(filename, 0644)) < 0) {
			exit(2);
		}

		if ((bytes = write(fd, buf, 26)) != 10) {
			exit(3);
		}
		exit(0);	/* success */
	}

	/* parent */
	if (waitpid(pid, &status, 0) == -1) {
		tst_brkm(TBROK, cleanup, "waitpid() failed");
	}
	switch (WEXITSTATUS(status)) {
	case 0:
		tst_resm(TPASS, "RLIMIT_FSIZE test PASSED");
		break;
	case 1:
		tst_resm(TFAIL, "setrlimit failed to set "
			 "RLIMIT_FSIZE, errno = %d", errno);
		break;
	case 2:
		tst_resm(TFAIL, "creating testfile failed");
		break;
	case 3:
		tst_resm(TFAIL, "setrlimit failed, expected "
			 "10 got %d", bytes);
		break;
	default:
		tst_resm(TFAIL, "child returned bad exit status");
	}
}

/*
 * test3 - Test for RLIMIT_NPROC
 */
void test3()
{
	if (getrlimit(RLIMIT_NPROC, &save_rlim) < 0) {
		tst_brkm(TBROK, cleanup, "getrlimit failed, errno: %d", errno);
	}

	rlim.rlim_cur = 10;
	rlim.rlim_max = 10;

	TEST(setrlimit(RLIMIT_NPROC, &rlim));

	if (TEST_RETURN == -1) {
		tst_resm(TFAIL, "setrlimit failed to set "
			 "RLIMIT_NPROC, errno = %d", errno);
		return;
	}

	if (!STD_FUNCTIONAL_TEST) {
		tst_resm(TPASS, "call succeeded");
		return;
	}

	if ((getrlimit(RLIMIT_NPROC, &rlim1)) == -1) {
		tst_brkm(TBROK, cleanup, "getrlimit failed to get "
			 "values for RLIMIT_NPROC, errno = %d", errno);
	}

	if ((rlim1.rlim_cur != 10) && (rlim1.rlim_max != 10)) {
		tst_resm(TFAIL, "setrlimit did not set the proc "
			 "limit correctly");
		return;
	}

	for (i = 0; i < 20; i++) {
		if ((pid = FORK_OR_VFORK()) == -1) {
			if (errno != EAGAIN) {
				tst_resm(TWARN, "Expected EAGAIN got %d",
					 errno);
			}
		} else if (pid == 0) {
			exit(0);
		}
	}
	waitpid(pid, &status, 0);
	if (WEXITSTATUS(status) != 0) {
		tst_resm(TFAIL, "RLIMIT_NPROC functionality is not correct");
	} else {
		tst_resm(TPASS, "RLIMIT_NPROC functionality is correct");
	}
}

/*
 * test4() - Test for RLIMIT_CORE by forking a child and
 *           having it cause a segfault
 */
void test4()
{
	rlim.rlim_cur = 10;
	rlim.rlim_max = 10;

	TEST(setrlimit(RLIMIT_CORE, &rlim));

	if (TEST_RETURN == -1) {
		tst_resm(TFAIL, "setrlimit failed to set "
			 "RLIMIT_CORE, ernro = %d", errno);
		return;
	}

	if (!STD_FUNCTIONAL_TEST) {
		tst_resm(TPASS, "call succeeded");
		return;
	}

	if ((pid = FORK_OR_VFORK()) == -1) {
		tst_brkm(TBROK, cleanup, "fork() failed");
	}

	if (pid == 0) {		/* child */
		char *testbuf = NULL;
		strcpy(testbuf, "abcd");
		exit(0);
	}
	wait(&status);

	if (access("core", F_OK) == 0) {
		tst_resm(TFAIL, "core dump was succesful "
			 "though it was not supposed to");
		return;
	} else if (errno != ENOENT) {
		tst_resm(TFAIL, "Expected ENOENT got %d", errno);
		return;
	}

	tst_resm(TPASS, "RLIMIT_CORE functionality is correct");
}

/*
 * sighandler() - catch sigsegv when generated by child in test #4
 */
void sighandler(int sig)
{
	if (sig != SIGSEGV && sig != SIGXFSZ) {
		tst_resm(TWARN, "caught signal %d, not SIGSEGV", sig);
		exit(1);
	}
	exit(0);
}

/*
 * setup() - performs all ONE TIME setup for this test.
 */
void setup()
{
	/* must run test as root */
	if (geteuid() != 0) {
		tst_brkm(TBROK, tst_exit, "Must run test as root");
	}

	umask(0);

	/* capture signals */
	tst_sig(FORK, sighandler, cleanup);

	/* Pause if that option was specified */
	TEST_PAUSE;

	/* make a temporary directory and cd to it */
	tst_tmpdir();

	sprintf(filename, "setrlimit1.%d", getpid());
}

/*
 * cleanup() - performs all ONE TIME cleanup for this test at
 *	       completion or premature exit.
 */
void cleanup(void)
{
	/*
	 * print timing status if that option was specified.
	 * print errno log if that option was specified
	 */
	TEST_CLEANUP;

	/* Remove tmp dir and all files in it */
	unlink(filename);
	tst_rmdir();

	/* exit with return code appropriate for results */
	tst_exit();
}
