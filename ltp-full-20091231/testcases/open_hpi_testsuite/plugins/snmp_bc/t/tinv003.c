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
	SaHpiRptEntryT rptentry;
	SaHpiRdrT	rdr;			
	SaHpiResourceIdT  id = 0;
        SaHpiSessionIdT sessionid;
	SaHpiIdrIdT        idrId= 0;
	SaHpiEntryIdT     areaId = 0;
	/* SaHpiIdrAreaTypeT areatype; */
	SaHpiEntryIdT     nextAreaId;
	SaHpiIdrAreaHeaderT header;
	SaHpiEntryIdT entryid;
	SaHpiEntryIdT nextentryid;
	SaHpiBoolT foundControl;			

	/* *************************************	 	 
	 * Find a resource with inventory capability
	 * ************************************* */
	err = tsetup(&sessionid);
	if (err != SA_OK) {
		printf("Error! Can not open session for test environment\n");
		printf("       File=%s, Line=%d\n", __FILE__, __LINE__);
		return -1;
	}

	err = tfind_resource(&sessionid, SAHPI_CAPABILITY_INVENTORY_DATA, SAHPI_FIRST_ENTRY, &rptentry, SAHPI_TRUE);
	if (err != SA_OK) {
		printf("Can not find an Inventory resource for test environment\n");
		printf("       File=%s, Line=%d\n", __FILE__, __LINE__);
		err = tcleanup(&sessionid);
		return SA_OK;
	}	
	id = rptentry.ResourceId;

	/************************** 
	 * Test: find an Inventory RDR
	 **************************/
	entryid = SAHPI_FIRST_ENTRY;
	foundControl = SAHPI_FALSE;			
	do {
		err = saHpiRdrGet(sessionid,id,entryid,&nextentryid, &rdr);
		if (err == SA_OK)
		{
			if (rdr.RdrType == SAHPI_INVENTORY_RDR) 
			{
				foundControl = SAHPI_TRUE;
				break;
														
			}
			entryid = nextentryid;
		}
	} while ((err == SA_OK) && (entryid != SAHPI_LAST_ENTRY)) ;

	if (!foundControl) {
		err("Did not find desired resource for test\n");
		return(SA_OK);
	} else {
		idrId = rdr.RdrTypeUnion.InventoryRec.IdrId; 
	}	
		
	/************************** 
	 * Test: Invalid IdrId
	 **************************/
	expected_err = SA_ERR_HPI_NOT_PRESENT;                   
	err = saHpiIdrAreaHeaderGet(sessionid , id, 5000,
		       			SAHPI_IDR_AREATYPE_UNSPECIFIED,
				       	areaId, &nextAreaId, &header);
	checkstatus(err, expected_err, testfail);
		
	/************************** 
	 * Test: Invalid AreaType
	 **************************/
	expected_err = SA_OK;                   
	err = saHpiIdrAreaHeaderGet(sessionid , id, idrId,
		       			SAHPI_IDR_AREATYPE_UNSPECIFIED,
				       	areaId, &nextAreaId, &header);
	checkstatus(err, expected_err, testfail);
		
	/************************** 
	 * Test: Normal code path
 	 **************************/
	expected_err = SA_OK;                   
	err = saHpiIdrAreaHeaderGet(sessionid , id, idrId,
		       			SAHPI_IDR_AREATYPE_UNSPECIFIED,
				       	SAHPI_FIRST_ENTRY, &nextAreaId, &header);
	checkstatus(err, expected_err, testfail);

	/**************************&*
	 * Cleanup after all tests
	 ***************************/
	err = tcleanup(&sessionid);
	return testfail;

}

#include <tsetup.c>
