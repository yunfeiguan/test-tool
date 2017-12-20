/*      -*- linux-c -*-
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
 *      Steve Sherman <stevees@us.ibm.com>
 */

#include <snmp_bc_plugin.h>

SaErrorT snmp_bc_get_watchdog_info(void *hnd,
				   SaHpiResourceIdT id,
				   SaHpiWatchdogNumT num,
				   SaHpiWatchdogT *wdt)
{
	/* Watchdog not supported */
        return SA_ERR_HPI_NOT_PRESENT;
}

SaErrorT snmp_bc_set_watchdog_info(void *hnd,
				   SaHpiResourceIdT id,
				   SaHpiWatchdogNumT num,
				   SaHpiWatchdogT *wdt)
{
	/* Watchdog not supported */
        return SA_ERR_HPI_NOT_PRESENT;
}

SaErrorT snmp_bc_reset_watchdog(void *hnd,
				SaHpiResourceIdT id,
				SaHpiWatchdogNumT num)
{
 	/* Watchdog not supported */
        return SA_ERR_HPI_NOT_PRESENT;
}


void * oh_get_watchdog_info (void *, SaHpiResourceIdT, SaHpiWatchdogNumT,
                             SaHpiWatchdogT *)
                __attribute__ ((weak, alias("snmp_bc_get_watchdog_info")));

void * oh_set_watchdog_info (void *, SaHpiResourceIdT, SaHpiWatchdogNumT,
                             SaHpiWatchdogT *)
                __attribute__ ((weak, alias("snmp_bc_set_watchdog_info")));
		
void * oh_reset_watchdog (void *, SaHpiResourceIdT, SaHpiWatchdogNumT)
                __attribute__ ((weak, alias("snmp_bc_reset_watchdog")));


