/*
 * crash02.c - Test OS robustness by executing syscalls with random args.
 *
 * Copyright (C) 2001 Stephane Fillod <f4cfe@free.fr>
 *
 * This test program was inspired from crashme, by GEORGE J. CARRETT.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

/*
A signal handler is set up so that in most cases the machine exception
generated by the illegal syscall, bad operands, etc in the procedure
made up of random data are caught; and another round of randomness may
be tried. Eventually a random syscall may corrupt the program or
the machine state in such a way that the program must halt. This is
a test of the robustness of the hardware/software for instruction
fault handling.

Note: Running this program just a few times, using total CPU time of
less than a few seconds SHOULD NOT GIVE YOU ANY CONFIDENCE in system
robustness. Having it run for hours, with tens of thousands of cases
would be a different thing. It would also make sense to run this
stress test at the same time you run other tests, like a multi-user
benchmark.

CAUTION: running this program may crash your system, your disk and all
	your data along! DO NOT RUN IT ON PRODUCTION SYSTEMS!
	CONSIDER YOUR DISK FRIED.
	REMEMBER THE DISCLAIMER PART OF THE LICENSE.

	Running as user nobody and with all your filesystems
	remounted to readonly may be wise..


TODO:
	* in rand_long(), stuff in some real pointers to random data
	* Does a syscall is supposed to send SIGSEGV?
*/


#define _GNU_SOURCE
#include <sys/syscall.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <setjmp.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "test.h"
#include "usctest.h"

char *TCID="crash02";
int TST_TOTAL=1;
extern int Tst_count;

static int x_opt = 0;
static int v_opt = 0;
static char *v_copt;
static int s_opt = 0;
static char *s_copt;
static int l_opt = 0;
static char *l_copt;
static int n_opt = 0;
static char *n_copt;

int verbose_level = 2;

/* depends on architecture.. */
unsigned int sysno_max = 127;

int nseed;
int ntries = 100;

/* max time allowed per try, in seconds */
#define MAX_TRY_TIME 5



void cleanup()
{
	/*
	 * remove the tmp directory and exit
	 */

	TEST_CLEANUP;

	tst_rmdir();

	tst_exit();
}

void setup()
{
	/*
	 * setup a default signal hander and a
	 * temporary working directory.
	 */
	tst_sig(FORK, DEF_HANDLER, cleanup);

	TEST_PAUSE;

	tst_tmpdir();
}

void help()
{
	printf("  -x      dry run, hexdump random code instead\n");
	printf("  -l x    max syscall no\n");
	printf("  -v x    verbose level\n");
	printf("  -s x    random seed\n");
	printf("  -n x    ntries\n");
}

/*
 */
option_t options[] =
{
	{ "v:", &v_opt, &v_copt },
	{ "l:", &l_opt, &l_copt },
	{ "s:", &s_opt, &s_copt },
	{ "n:", &n_opt, &n_copt },
	{ "x", &x_opt, NULL },

	{ NULL, NULL, NULL }
};


void badboy_fork ();
void badboy_loop ();

void summarize_errno ();
void record_errno(unsigned int n);


int
main (int argc, char *argv[])
{
  char *msg;
  int lc;

  if ( (msg=parse_opts(argc, argv, options, help)) != (char *) NULL )
	tst_brkm(TBROK, cleanup, "OPTION PARSING ERROR - %s", msg);

  if (v_opt)
	  verbose_level = atoi(v_copt);

  if (n_opt)
	  ntries = atoi(n_copt);

  if (l_opt)
	  sysno_max = atoi(l_copt);

  if (s_opt)
	  nseed = atoi(s_copt);
  else
	  nseed = time(NULL);

   setup();

   for (lc=0; TEST_LOOPING(lc); lc++)
    {
		Tst_count=0;

		tst_resm(TINFO, "crashme02 %d %d %d",
	   		sysno_max, nseed, ntries);

  		srand (nseed);
		badboy_fork ();

		/* still there? */
		tst_resm(TPASS, "we're still here, OS seems to be robust");

		nseed++;
	}
	cleanup();
	return 0;
}

/* ************************* */
int badboy_pid;

void my_signal (int sig, void (*func) ());


void monitor_fcn (int sig)
{
  int status;

 if (verbose_level >= 3) 
	    printf ("time limit reached on pid. using kill.\n");

  status = kill (badboy_pid, SIGKILL);
  if (status < 0)
    {
	if (verbose_level >= 3) 
      		printf ("failed to kill process\n");
    }
}


void
badboy_fork ()
{
	int status, pid;

      status = fork ();
      badboy_pid = status;
      if (status == 0)	/* badboy */
	{
#ifdef DEBUG_LATE_BADBOY
	  sleep(ntries*MAX_TRY_TIME+10);
#else
	  badboy_loop ();
#endif
	  exit (0);	/* tough guy */
	}
      else if (status < 0)
	perror ("fork");
      else			/* parent watching over badboy */
	{
	  if (verbose_level > 3)
	  	printf ("badboy pid = %d\n", badboy_pid);

/* don't trust the child to return at night */
      my_signal (SIGALRM, monitor_fcn);
      alarm (ntries*MAX_TRY_TIME);

	  pid = waitpid (-1, &status, WUNTRACED);
	  if (pid <= 0)
	  {
		  perror ("wait");
	  } else {
	  	if (verbose_level > 3)
		  printf ("pid %d exited with status %d\n", pid, status);
		#if 0
		record_status(status);
		#endif
	   }
	}	/* parent */
	alarm(0);
}

/* *************** status recording ************************* */

/* errno status table (max is actually around 127) */
#define STATUS_MAX 256
static int errno_table[STATUS_MAX];

void record_errno(unsigned int n)
{
	if (n >= STATUS_MAX)
		return;

	errno_table[n]++;
}

/* may not work with -c option */
void
summarize_errno ()
{
  int i;

  if (x_opt || verbose_level < 2) 
		  return;

  printf ("errno status ... number of cases\n");
  for (i = 0; i < STATUS_MAX; i++)
    {
      if (errno_table[i])
      	printf ( "%12d ... %5d\n", i, errno_table[i]);
    }
}


/* ************* badboy ******************************************* */

jmp_buf again_buff;


unsigned char * bad_malloc (int n);
void my_signal (int sig, void (*func) ());
void again_handler (int sig);
void try_one_crash (int try_num);
void set_up_signals ();
int in_blacklist (int sysno);

/* badboy "entry" point */

/*
 * Unlike crashme, faulty syscalls are not supposed to barf
 */
void
badboy_loop ()
{
  int i;


  for (i = 0; i < ntries; ++i)
    {
	  /* level 5 */

  if (!x_opt && verbose_level >= 5) {
		printf ("try %d\n", i);
  }

      if (setjmp (again_buff) == 3)
	{
  	  if (verbose_level >= 5)
	  	printf ("Barfed\n");
	}
      else
	{
	  set_up_signals ();
	  alarm (MAX_TRY_TIME);
	  try_one_crash (i);
	}
  }
  summarize_errno();
}

void again_handler (int sig)
{
  char *ss;

  switch (sig)
    {
    case SIGILL:
      ss = " illegal instruction";
      break;
#ifdef SIGTRAP
    case SIGTRAP:
      ss = " trace trap";
      break;
#endif
    case SIGFPE:
      ss = " arithmetic exception";
      break;
#ifdef SIGBUS
    case SIGBUS:
      ss = " bus error";
      break;
#endif
    case SIGSEGV:
      ss = " segmentation violation";
      break;
#ifdef SIGIOT
    case SIGIOT:
      ss = " IOT instruction";
      break;
#endif
#ifdef SIGEMT
    case SIGEMT:
      ss = " EMT instruction";
      break;
#endif
#ifdef SIGALRM
    case SIGALRM:
      ss = " alarm clock";
      break;
#endif
    case SIGINT:
      ss = " interrupt";
      break;
    default:
      ss = "";
    }
  if (verbose_level >= 5) 
  	printf ("Got signal %d%s\n", sig, ss);

  longjmp (again_buff, 3);
}

void my_signal (int sig, void (*func) ())
{
  struct sigaction act;

  act.sa_handler = func;
  memset (&act.sa_mask, 0x00, sizeof (sigset_t));
  act.sa_flags = SA_NOMASK|SA_RESTART;
  sigaction (sig, &act, 0);
}

void
set_up_signals ()
{
  my_signal (SIGILL, again_handler);
#ifdef SIGTRAP
  my_signal (SIGTRAP, again_handler);
#endif
  my_signal (SIGFPE, again_handler);
#ifdef SIGBUS
  my_signal (SIGBUS, again_handler);
#endif
  my_signal (SIGSEGV, again_handler);
#ifdef SIGIOT
  my_signal (SIGIOT, again_handler);
#endif
#ifdef SIGEMT
  my_signal (SIGEMT, again_handler);
#endif
#ifdef SIGALRM
  my_signal (SIGALRM, again_handler);
#endif
  my_signal (SIGINT, again_handler);
}

/*
 * NB: rand() (ie. RAND_MAX) might be on 31bits only!
 *
 * FIXME: 64bits systems
 *
 * TODO: improve arg mixing (16bits and 8bits values, NULLs, etc.).
 *	big values as returned by rand() are no so interresting 
 *	(except when used as pointers) because they may fall too 
 *	quickly in the invalid parameter sieve. Smaller values,
 *	will be more insidious because they may refer to existing
 *	objects (pids, fd, etc.).
 */
long int rand_long()
{
    int r1, r2;

    r1 = rand();
    r2 = rand();
    
    if (r1 & 0x10000L)
    	r1 = 0;
    if (!r1 && (r2 & 0x50000L))
    	r2 = 0;
    else if (!r1 && (r2 & 0x20000L))
    	r2 &= 0x00ffL;

    return (long int) ((r1 & 0xffffL) << 16) | (r2 & 0xffffL);
}

void
try_one_crash (int try_num)
{
  long int sysno, arg1, arg2, arg3, arg4, arg5, arg6, arg7;

  do {
    sysno = rand()%sysno_max;
  } while (in_blacklist(sysno));

  arg1 = rand_long();
  arg2 = rand_long();
  arg3 = rand_long();
  arg4 = rand_long();
  arg5 = rand_long();
  arg6 = rand_long();
  arg7 = rand_long();

  if (x_opt) {
      if (verbose_level >= 1) 
  	printf("%04d: syscall(%ld, %#lx, %#lx, %#lx, %#lx, %#lx, %#lx, %#lx)\n",
		try_num, sysno, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
  } else {
    syscall (sysno, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
    record_errno(errno);
  }
}

/* The following syscalls create new processes which may cause the test
   unable to finish. */
int
in_blacklist (int sysno)
{
  int i;
  const list[] =
    {
#if defined(__ia64__)
      SYS_clone2,
#else
      /*
       * No SYS_fork(vfork) on IA-64. Instead, it uses,
       * clone(child_stack=0, flags=CLONE_VM|CLONE_VFORK|SIGCHLD)
       * clone2()
       */
      
      /*
       * NOTE (garrcoop):
       *  Could not find reference to SYS_fork(vfork) on mips32
       *  with the Montavista / Octeon toolchain. Need to develop an
       *  autoconf check for this item.
       */   
#if defined(__NR_vfork) && __NR_vfork
      SYS_vfork,
#endif
#if defined(__NR_fork) && __NR_fork
      SYS_fork,
#endif
#endif /* __ia64__ */
#if defined(__NR_clone) && __NR_clone
      SYS_clone,
#endif
      -1
    };

  for (i = 0; list[i] != -1; i++)
    {
      if (sysno == list[i])
        return 1;
    }

  return 0;
}
