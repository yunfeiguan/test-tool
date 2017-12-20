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
 * main: Starts with an RPTable of 1 resource, adds 1 rdr with data
 * to first resource. Fetches data using a Resource Id not present in the table.
 * Success if the interface returns an error, otherwise there was a failure.
 *
 * Return value: 0 on success, 1 on failure
 **/
int main(int argc, char **argv)
{
        RPTable *rptable = (RPTable *)g_malloc0(sizeof(RPTable));
        oh_init_rpt(rptable);
        gchar *data = "My data";

        if (oh_add_resource(rptable, rptentries, NULL, 1))
                return 1;

        if (oh_add_rdr(rptable, rptentries[0].ResourceId, sensors, data, 1))
                return 1;

        if (oh_get_rdr_data(rptable, 1234567, sensors[0].RecordId))
                return 1;

        return 0;
}
