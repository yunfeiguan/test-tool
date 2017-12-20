/*      -*- linux-c -*-
 *
 * (C) Copyright IBM Corp. 2004
 * Copyright (c) 2004 by Intel Corp.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  This
 * file and program are licensed under a BSD style license.  See
 * the Copying file included with the OpenHPI distribution for
 * full licensing terms.
 *
 * Authors:
 *      Christina Hernandez<hernanc@us.ibm.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include <string.h>

#include <SaHpi.h>
#include <openhpi.h>
#include <oh_utils.h>
#include <el_utils.h>


#include "el_test.h"

/**
 * main: EL test
 *
 * This test creates an EL and resets info.OverflowFlag (oh_el_overflowreset)
 * Return value: 0 on success, 1 on failure
 **/


int main(int argc, char **argv)
{
        oh_el *el;
        SaErrorT retc;
	
	/* map el from file*/
	el = oh_el_create(20);
	retc = oh_el_map_from_file(el, "./elTest.data");
	if (retc != SA_OK) {
                err("ERROR: oh_el_map_from_file failed.");
                return 1;
        }

	retc = oh_el_overflowreset(el);
        if (retc != SA_OK) {
                err("ERROR: el info.OverflowFlagreset failed.");
                return 1;
        }

        return 0;
}

