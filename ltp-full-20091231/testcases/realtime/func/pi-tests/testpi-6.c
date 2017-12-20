/******************************************************************************
 *
 *   Copyright © International Business Machines  Corp., 2008
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
 *
 * NAME
 *      testpi-6.c
 *
 * DESCRIPTION
 *
 *
 *
 * USAGE:
 *      Use run_auto.sh script in current directory to build and run test.
 *
 * AUTHOR
 *
 *
 * HISTORY
 *
 *
 *****************************************************************************/

#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <librttest.h>
pthread_mutex_t  child_mutex;

void* child_thread (void* arg)
{
    int ret;

    ret = pthread_mutex_lock(&child_mutex);
    if (ret != 0)
	printf("child thread: Failed to lock child_mutex: %d\n", ret);
    else
	printf("child_thread: got lock\n");

    sleep(2);

    printf("child_thread: Trying to get lock 2nd time\n");
    ret = pthread_mutex_lock(&child_mutex);
    if (ret != 0)
	printf("child thread: Failed to lock child_mutex: %d\n", ret);
    else
	printf("child_thread: got lock 2nd time !!\n");

    return NULL;
}

int do_test(int argc, char **argv)
{
    pthread_mutexattr_t mutexattr;
    int retc, robust;

    if (pthread_mutexattr_init(&mutexattr) != 0) {
      printf("Failed to init mutexattr\n");
    };
    if (pthread_mutexattr_setrobust_np(&mutexattr, PTHREAD_MUTEX_ROBUST_NP) != 0) {
      printf("Can't set robust mutex\n");
    }
    if (pthread_mutexattr_getrobust_np(&mutexattr, &robust) != 0) {
      printf("Can't get mutexattr protocol\n");
    } else {
      printf("robust in mutexattr is %d\n", robust);
    }
    if ((retc = pthread_mutex_init(&child_mutex, &mutexattr)) != 0) {
      printf("Failed to init mutex: %d\n", retc);
    }

    create_other_thread(child_thread, NULL);
    join_threads();

    return 0;
}

#include "test-skeleton.c"
