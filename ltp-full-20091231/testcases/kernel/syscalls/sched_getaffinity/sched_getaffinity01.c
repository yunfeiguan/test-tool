/******************************************************************************/
/* Copyright (c) Crackerjack Project., 2007                                   */
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
/******************************************************************************/
/*                                                                            */
/* File:        sched_getaffinity01.c                                         */
/*                                                                            */
/* Description: This tests the sched_getaffinity() syscall                    */
/*                                                                            */
/* Usage:  <for command-line>                                                 */
/* sched_getaffinity01 [-c n] [-e][-i n] [-I x] [-p x] [-t]                   */
/*      where,  -c n : Run n copies concurrently.                             */
/*              -e   : Turn on errno logging.                                 */
/*              -i n : Execute test n times.                                  */
/*              -I x : Execute test for x seconds.                            */
/*              -P x : Pause for x seconds between iterations.                */
/*              -t   : Turn on syscall timing.                                */
/*                                                                            */
/* Total Tests: 1                                                             */
/*                                                                            */
/* Test Name:   sched_getaffinity01                                           */
/* History:     Porting from Crackerjack to LTP is done by                    */
/*              Manas Kumar Nayak maknayak@in.ibm.com>                        */
/******************************************************************************/
#include<stdlib.h>
#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#define _GNU_SOURCE 
#define __USE_GNU
#include<sched.h>
#include<string.h>
#include <errno.h>

/* Harness Specific Include Files. */
#include "test.h"
#include "usctest.h"
#include "linux_syscall_numbers.h"

/* Extern Global Variables */
extern int Tst_count;           /* counter for tst_xxx routines.         */
extern char *TESTDIR;           /* temporary dir created by tst_tmpdir() */

/* Global Variables */
char *TCID = "sched_getaffinity01";  /* Test program identifier.*/
int  testno;
int  TST_TOTAL = 1;                   /* total number of tests in this file.   */

/* Extern Global Functions */
/******************************************************************************/
/*                                                                            */
/* Function:    cleanup                                                       */
/*                                                                            */
/* Description: Performs all one time clean up for this test on successful    */
/*              completion,  premature exit or  failure. Closes all temporary */
/*              files, removes all temporary directories exits the test with  */
/*              appropriate return code by calling tst_exit() function.       */
/*                                                                            */
/* Input:       None.                                                         */
/*                                                                            */
/* Output:      None.                                                         */
/*                                                                            */
/* Return:      On failure - Exits calling tst_exit(). Non '0' return code.   */
/*              On success - Exits calling tst_exit(). With '0' return code.  */
/*                                                                            */
/******************************************************************************/
extern void cleanup() {
        /* Remove tmp dir and all files in it */
        TEST_CLEANUP;
        tst_rmdir();

        /* Exit with appropriate return code. */
        tst_exit();
}

/* Local  Functions */
/******************************************************************************/
/*                                                                            */
/* Function:    setup                                                         */
/*                                                                            */
/* Description: Performs all one time setup for this test. This function is   */
/*              typically used to capture signals, create temporary dirs      */
/*              and temporary files that may be used in the course of this    */
/*              test.                                                         */
/*                                                                            */
/* Input:       None.                                                         */
/*                                                                            */
/* Output:      None.                                                         */
/*                                                                            */
/* Return:      On failure - Exits by calling cleanup().                      */
/*              On success - returns 0.                                       */
/*                                                                            */
/******************************************************************************/
void setup() {
        /* Capture signals if any */
        /* Create temporary directories */
        TEST_PAUSE;
        tst_tmpdir();
}

#define QUICK_TEST(t) \
do { \
	TEST(t); \
	tst_resm((TEST_RETURN == -1 ? TPASS : TFAIL) | TTERRNO, #t); \
} while (0)

int main(int ac, char **av) {
        int lc,num,i;                 /* loop counter */
        char *msg;              /* message returned from parse_opts */
	cpu_set_t mask;
	unsigned int len = sizeof(cpu_set_t);

        /* parse standard options */
        if ((msg = parse_opts(ac, av, (option_t *)NULL, NULL)) != (char *)NULL){
             tst_brkm(TBROK, cleanup, "OPTION PARSING ERROR - %s", msg);
             tst_exit();
           }

        setup();

	TEST(num=sysconf(_SC_NPROCESSORS_CONF));  //the number of processor(s)
        tst_resm(TINFO,"system has %d processor(s).", num);

        /* Check looping state if -i option given */
        for (lc = 0; TEST_LOOPING(lc); ++lc) {
                Tst_count = 0;
                for (testno = 0; testno < TST_TOTAL; ++testno) {
                     QUICK_TEST(sched_getaffinity(0, len, (cpu_set_t *)-1));
                     QUICK_TEST(sched_getaffinity(0, 0, &mask));
                     QUICK_TEST(sched_getaffinity(getpid() + 1, len, &mask));

		     CPU_ZERO(&mask); //clear
                     TEST(sched_getaffinity(0,len,&mask));     //call sched_getaffinity()
		     if(TEST_RETURN == -1) {
                 	   tst_resm(TFAIL|TTERRNO, "could not get cpu affinity");
			cleanup();
			tst_exit();
                     } else {
				tst_resm(TINFO,"cpusetsize is %d", len);
				tst_resm(TINFO,"mask.__bits[0] = %lu ",mask.__bits[0]);
				for(i=0;i<num;i++){    // check the processor
	                        	TEST(CPU_ISSET(i,&mask));
					if (TEST_RETURN != -1 ){
                                	tst_resm(TPASS,"sched_getaffinity() succeed ,this process %d is running processor: %d",getpid(), i);
					}
			        }
	                    }


                     }
		}
        cleanup();
	tst_exit();
}

