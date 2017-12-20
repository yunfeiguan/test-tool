/*      -*- linux-c -*-
 *
 *(C) Copyright IBM Corp. 2005
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  This
 * file and program are licensed under a BSD style license.  See
 * the Copying file included with the OpenHPI distribution for
 * full licensing terms.
 *
 *	Authors:
 *     	Sean Dague <http://dague.net/sean>
*/

#include <stdlib.h>
#include <SaHpi.h>
#include <oh_utils.h>
#include <sahpi_struct_utils.h>
#include <oh_error.h>


/**
 * Run a series of sanity tests on the simulator
 * Return 0 on success, otherwise return -1
 **/


static SaHpiResourceIdT get_resid(SaHpiSessionIdT sid,
                           SaHpiEntryIdT srchid) {
        SaHpiRptEntryT res;
        SaHpiEntryIdT rptid = SAHPI_FIRST_ENTRY;

        while(saHpiRptEntryGet(sid, rptid, &rptid, &res) == SA_OK) {
                if (srchid == res.ResourceEntity.Entry[0].EntityType) {
                        return res.ResourceId;
                }
        }
        return 0;
}


int main(int argc, char **argv)
{
	SaHpiSessionIdT sid = 0;
        SaHpiWatchdogT wd;
	SaErrorT rc = SA_OK;

        rc = saHpiSessionOpen(SAHPI_UNSPECIFIED_DOMAIN_ID, &sid, NULL);
	if (rc != SA_OK) {
		err("Failed to open session");
                return -1;
	}

	rc = saHpiDiscover(sid);
	if (rc != SA_OK) {
		err("Failed to run discover");
                return -1;
	}

        /* get the resource id of the chassis */
        SaHpiResourceIdT resid = get_resid(sid, SAHPI_ENT_SYSTEM_CHASSIS);
        if (resid == 0) {
		err("Couldn't find the resource id of the chassis");
                return -1;
	}

        rc = saHpiWatchdogTimerGet(sid, resid, 1, &wd);
        if (rc != SA_OK) {
		err("Couldn't get watchdog timer");
		err("Error %s",oh_lookup_error(rc));
                return -1;
	}

        rc = saHpiWatchdogTimerSet(sid, resid, 1, &wd);
        if (rc != SA_OK) {
		err("Couldn't set watchdog timer");
		err("Error %s",oh_lookup_error(rc));
                return -1;
	}

	return 0;
}

