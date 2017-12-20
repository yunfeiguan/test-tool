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


#ifndef Included_oSaHpiIdrInfo
#define Included_oSaHpiIdrInfo

#include <stdio.h>
extern "C"
{
#include <SaHpi.h>
}


class oSaHpiIdrInfo : public SaHpiIdrInfoT {
    public:
        // constructors
        oSaHpiIdrInfo();
        // copy constructor
        oSaHpiIdrInfo(const oSaHpiIdrInfo& buf);
        // destructor
        ~oSaHpiIdrInfo() {
        }
        // other methods
        static bool assignField(SaHpiIdrInfoT * ptr,
                                const char *field,
                                const char *value);
        inline bool assignField(const char *field,
                                const char *value) {
            return assignField(this, field, value);
        }
        inline SaHpiIdrInfoT *getStruct(void) {
            return this;
        }
        static bool fprint(FILE *stream,
                           const int indent,
                           const SaHpiIdrInfoT *buffer);
        inline bool fprint(FILE *stream,
                           const int indent) {
            return fprint(stream, indent, this);
        }
};

#endif

