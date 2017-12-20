/* -*- linux-c -*-
 * 
 * (C) Copyright IBM Corp. 2004
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  This
 * file and program are licensed under a BSD style license.  See
 * the Copying file included with the OpenHPI distribution for
 * full licensing terms.
 *
 * Author(s):
 *     Peter D Phan <pdphan@users.sourceforge.net>
 */


#include <snmp_bc_plugin.h>
#include <sahpimacros.h>
#include <tsetup.h>

int main(int argc, char **argv) 
{

	/* ************************
	 * Local variables
	 * ***********************/	 
	int testfail = 0;
	SaErrorT          err;
	SaErrorT expected_err;
					
	SaHpiResourceIdT  id = 0;
        SaHpiSessionIdT sessionid;
	SaHpiEventLogEntryIdT previd;
	SaHpiEventLogEntryIdT nextid;
	SaHpiEventLogEntryT   entry;
	SaHpiRdrT             rdr;
	SaHpiRptEntryT        rptentry;
	
	/* *************************************	 	 
	 * Find a resource with EventLog type rdr
	 * ************************************* */
	err = tsetup(&sessionid);
	if (err != SA_OK) {
		printf("Error! Can not open session for test environment\n");
		printf("       File=%s, Line=%d\n", __FILE__, __LINE__);
		return -1;
	}
	err = tfind_resource(&sessionid, SAHPI_CAPABILITY_EVENT_LOG, SAHPI_FIRST_ENTRY, &rptentry, SAHPI_TRUE);
	if (err != SA_OK) {
		printf("Can not find a control resource for test environment\n");
		printf("       File=%s, Line=%d\n", __FILE__, __LINE__);
		err = tcleanup(&sessionid);
		return SA_OK;
	}
	
	id = rptentry.ResourceId;
	/************************** 
	 * Test 
	 **************************/
	expected_err = SA_OK;                   
	err = saHpiEventLogEntryGet(sessionid, id, SAHPI_NEWEST_ENTRY, &previd, &nextid, &entry, &rdr, &rptentry);
	checkstatus(err, expected_err, testfail);

	/************************** 
	 * Test: NULL rdr
	 * expected_err = SA_OK;                   
	 **************************/
	err = saHpiEventLogEntryGet(sessionid, id, SAHPI_OLDEST_ENTRY, &previd, &nextid, &entry, NULL, &rptentry);
	checkstatus(err, expected_err, testfail);

	/************************** 
	 * Test: NULL rpt 
	 * expected_err = SA_OK;                   
	 **************************/
	err = saHpiEventLogEntryGet(sessionid, id, SAHPI_NEWEST_ENTRY, &previd, &nextid, &entry, &rdr, NULL);
	checkstatus(err, expected_err, testfail);

	/************************** 
	 * Test: NULL rpt and rdr 
	 * expected_err = SA_OK;                   
	 **************************/
	err = saHpiEventLogEntryGet(sessionid, id, SAHPI_OLDEST_ENTRY, &previd, &nextid, &entry, NULL, NULL); 
	checkstatus(err, expected_err, testfail);


	/***************************
	 * Cleanup after all tests
	 ***************************/
	err = tcleanup(&sessionid);
	return testfail;

}

#include <tsetup.c>
