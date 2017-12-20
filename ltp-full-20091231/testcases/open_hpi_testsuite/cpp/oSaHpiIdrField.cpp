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
#include "oSaHpiTypesEnums.hpp"
#include "oSaHpiTextBuffer.hpp"
#include "oSaHpiIdrField.hpp"


/**
 * Default constructor.
 */
oSaHpiIdrField::oSaHpiIdrField() {
    AreaId = 1;
    FieldId = 1;
    Type = SAHPI_IDR_FIELDTYPE_UNSPECIFIED;
    ReadOnly = false;
    Field.DataType = SAHPI_TL_TYPE_TEXT;
    Field.Language = SAHPI_LANG_ENGLISH;
    Field.DataLength = 0;
    Field.Data[0] = '\0';
};


/**
 * Constructor.
 *
 * @param buf    The reference to the class to be copied.
 */
oSaHpiIdrField::oSaHpiIdrField(const oSaHpiIdrField& buf) {
    memcpy(this, &buf, sizeof(SaHpiIdrFieldT));
}



/**
 * Assign a field in the SaHpiIdrFieldT struct a value.
 *
 * @param field  The pointer to the struct (class).
 * @param field  The field name as a text string (case sensitive).
 * @param value  The character string value to be assigned to the field. This
 *               value will be converted as necessary.
 *
 * @return True if there was an error, otherwise false.
 */
bool oSaHpiIdrField::assignField(SaHpiIdrFieldT *ptr,
                                 const char *field,
                                 const char *value) {
    // note that DataLength cannot be assigned a value using this method
    if (ptr == NULL || field == NULL || value == NULL) {
        return true;
    }
    if (strcmp(field, "AreaId") == 0) {
        ptr->AreaId = strtoul(value, NULL, 10);
        return false;
    }
    else if (strcmp(field, "FieldId") == 0) {
        ptr->FieldId = strtoul(value, NULL, 10);
        return false;
    }
    else if (strcmp(field, "Type") == 0) {
        ptr->Type = oSaHpiTypesEnums::str2idrfieldtype(value);
        return false;
    }
    else if (strcmp(field, "ReadOnly") == 0) {
        ptr->ReadOnly = oSaHpiTypesEnums::str2torf(value);
        return false;
    }
    // Field
    return true;
};


/**
 * Print the contents of the buffer.
 *
 * @param stream Target stream.
 * @param buffer Address of the SaHpiIdrFieldT struct.
 *
 * @return True if there was an error, otherwise false.
 */
bool oSaHpiIdrField::fprint(FILE *stream,
                            const int indent,
                            const SaHpiIdrFieldT *buffer) {
	int i, err;
    char indent_buf[indent + 1];

    if (stream == NULL || buffer == NULL) {
        return true;
    }
    for (i = 0; i < indent; i++) {
        indent_buf[i] = ' ';
    }
    indent_buf[indent] = '\0';

    err = fprintf(stream, "%s", indent_buf);
    if (err < 0) {
        return true;
    }
    err = fprintf(stream, "AreaId = %d\n", buffer->AreaId);
    if (err < 0) {
        return true;
    }
    err = fprintf(stream, "%s", indent_buf);
    if (err < 0) {
        return true;
    }
    err = fprintf(stream, "FieldId = %d\n", buffer->FieldId);
    if (err < 0) {
        return true;
    }
    err = fprintf(stream, "%s", indent_buf);
    if (err < 0) {
        return true;
    }
    err = fprintf(stream, "Type = %s\n", oSaHpiTypesEnums::idrfieldtype2str(buffer->Type));
    if (err < 0) {
        return true;
    }
    err = fprintf(stream, "%s", indent_buf);
    if (err < 0) {
        return true;
    }
    err = fprintf(stream, "ReadOnly = %s\n", oSaHpiTypesEnums::torf2str(buffer->ReadOnly));
    if (err < 0) {
        return true;
    }
    err = fprintf(stream, "%s", indent_buf);
    if (err < 0) {
        return true;
    }
    err = fprintf(stream, "Field\n");
    const SaHpiTextBufferT *tb = (const SaHpiTextBufferT *)&buffer->Field;
    err = oSaHpiTextBuffer::fprint(stream, indent + 3, tb);
    if (err < 0) {
        return true;
    }

	return false;
}

