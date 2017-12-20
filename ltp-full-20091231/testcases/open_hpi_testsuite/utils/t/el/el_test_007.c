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
 * This test modifies the timestamp basetime. 
 * It then adds a new entry and examines the 
 * timestamp to verify accuracy.
 *
 * Return value: 0 on success, 1 on failure
 **/


int main(int argc, char **argv)
{
        oh_el *el;
        SaErrorT retc;
	SaHpiEventLogInfoT info;
	SaHpiEventT event;
	SaHpiTimeT timestamp = 1000000000LL;
	static char *data[1] = {
        	"Test data"
	};

	/* create a new EL of size 20*/
	el = oh_el_create(20);

	/* add an event to el */
	event.Source = 1;
	event.EventType = SAHPI_ET_USER;
	event.Timestamp = SAHPI_TIME_UNSPECIFIED;
	event.Severity = SAHPI_DEBUG;

        strcpy((char *)event.EventDataUnion.UserEvent.UserEventData.Data, data[0]);

	/* modifies the timestamp basetime */
	retc = oh_el_timeset(el, timestamp);
	if (retc != SA_OK){
		err("ERROR: timeset failed");
		return 1;
	} 

	retc = oh_el_append(el, &event, NULL, NULL);
        if (retc != SA_OK) {
        	err("ERROR: oh_el_append failed.");
	        return 1;
        }
       
        /* get el info */
        retc = oh_el_info(el, &info);
        if (retc != SA_OK) {
                err("ERROR: oh_el_info failed.");
                return 1;
        }
									
	/* Verifies timestamp basetime worked */
	if (info.UpdateTimestamp < timestamp){
		err("ERROR: Timestamp basetime failed");
		return 1;
	}    

        /* close el without saving to file*/
        retc = oh_el_close(el);
        if (retc != SA_OK) {
                err("ERROR: oh_el_close on el failed.");
                return 1;
        }

        return 0;
}


