/*      -*- linux-c -*-
 *
 * (C) Copyright IBM Corp. 2005
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  This
 * file and program are licensed under a BSD style license.  See
 * the Copying file included with the OpenHPI distribution for
 * full licensing terms.
 *
 * Author(s):
 *    W. David Ashley <dashley@us.ibm.com>
 */


#include <stdlib.h>
#include <string.h>
#include <stdio.h>
extern "C"
{
#include <SaHpi.h>
}
#include "oSaHpiEventLogInfo.hpp"


int main(int argc, char *argv[]) {
    oSaHpiEventLogInfo *ptr1;

    // create the first EventLogInfo
    ptr1 = new oSaHpiEventLogInfo;
    if (ptr1 == NULL) {
        printf("Error: Unable to create a oSaHpiEventLogInfo.\n");
        return -1;
    }

    // print the contents of the first EventLogInfo
    fprintf(stdout, "\nSaHpiEventLogInfo\n");
    if (ptr1->fprint(stdout, 3)) {
        printf("Error: Unable to print the buffer.\n");
        return -1;

    }
    fprintf(stdout, "\n");

    printf("Success!\n");
    return 0;
}

