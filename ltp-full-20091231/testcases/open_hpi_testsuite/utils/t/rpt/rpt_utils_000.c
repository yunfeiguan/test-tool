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
 * Authors:
 *     Renier Morales <renier@openhpi.org>
 */

#include <glib.h>
#include <string.h>

#include <SaHpi.h>
#include <oh_utils.h>
#include <rpt_resources.h>

/**
 * main: Starting with an empty RPTable, adds 1 resource to it
 * and tries to fetch it by the ResourceId and compare it against
 * the original resource. A failed comparison means the test
 * failed, otherwise the test passed.
 *
 * Return value: 0 on success, 1 on failure
 **/
int main(int argc, char **argv)
{
        RPTable *rptable = (RPTable *)g_malloc0(sizeof(RPTable));
        oh_init_rpt(rptable);
        SaHpiRptEntryT *tmpentry = NULL;

        if (oh_add_resource(rptable, rptentries, NULL, 0))
                return 1;

        tmpentry = oh_get_resource_by_id(rptable, rptentries[0].ResourceId);
        if (!tmpentry || memcmp(rptentries, tmpentry, sizeof(SaHpiRptEntryT)))
                return 1;

        return 0;
}
