/*
 * Copyright (c) 2005 by IBM Corporation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  This
 * file and program are licensed under a BSD style license.  See
 * the Copying file included with the OpenHPI distribution for
 * full licensing terms.
 *
 * Authors:
 *     W. David Ashley <dashley@us.ibm.com>
 */

#include "marshal_hpi_types.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


static int
cmp_text_buffer( SaHpiTextBufferT *d1, SaHpiTextBufferT *d2 )
{
  if ( d1->DataType != d2->DataType )
       return 0;

  if ( d1->Language != d2->Language )
       return 0;

  if ( d1->DataLength != d2->DataLength )
       return 0;

  return memcmp( d1->Data, d2->Data, d1->DataLength ) ? 0 : 1;
}


static int
cmp_entity( SaHpiEntityT *d1, SaHpiEntityT *d2 )
{

  if ( d1->EntityType != d2->EntityType )
       return 0;

  if ( d1->EntityLocation != d2->EntityLocation )
       return 0;

  return 1;
}


static int
cmp_entitypath( SaHpiEntityPathT *d1, SaHpiEntityPathT *d2 )
{
  int i;

  for (i = 0; ; i++) {
      if (d1->Entry[i].EntityType == SAHPI_ENT_ROOT && d2->Entry[i].EntityType == SAHPI_ENT_ROOT) {
           return 1;
      }
      if ( cmp_entity(&d1->Entry[i], &d2->Entry[i]) == 0 )
           return 0;
  }

  return 1;
}


static int
cmp_name( SaHpiNameT *d1, SaHpiNameT *d2 )
{
  if ( d1->Length != d2->Length )
       return 0;

  if ( memcmp(&d1->Value, &d2->Value, SA_HPI_MAX_NAME_LENGTH) != 0 )
       return 0;

  return 1;
}


static int
cmp_condition( SaHpiConditionT *d1, SaHpiConditionT *d2 )
{
  if ( d1->Type != d2->Type )
       return 0;

  if ( !cmp_entitypath( &d1->Entity, &d2->Entity ) )
       return 0;

  if ( d1->DomainId != d2->DomainId )
       return 0;

  if ( d1->ResourceId != d2->ResourceId )
       return 0;

  if ( d1->SensorNum != d2->SensorNum )
       return 0;

  if ( d1->SensorNum != d2->SensorNum )
       return 0;

  if ( d1->EventState != d2->EventState )
       return 0;

  if ( !cmp_name( &d1->Name, &d2->Name ) )
       return 0;

  if ( d1->Mid != d2->Mid )
       return 0;

  if ( !cmp_text_buffer( &d1->Data, &d2->Data ) )
       return 0;

  return 1;
}


typedef struct
{
  tUint8 m_pad1;
  SaHpiConditionT m_v1;
  tUint8 m_pad2;
  SaHpiConditionT m_v2;
  SaHpiConditionT m_v3;
  tUint8 m_pad3;
} cTest;

cMarshalType StructElements[] =
{
  dStructElement( cTest, m_pad1 , Marshal_Uint8Type ),
  dStructElement( cTest, m_v1   , SaHpiConditionType ),
  dStructElement( cTest, m_pad2 , Marshal_Uint8Type ),
  dStructElement( cTest, m_v2   , SaHpiConditionType ),
  dStructElement( cTest, m_v3   , SaHpiConditionType ),
  dStructElement( cTest, m_pad3 , Marshal_Uint8Type ),
  dStructElementEnd()
};

cMarshalType TestType = dStruct( cTest, StructElements );


int
main( int argc, char *argv[] )
{
  cTest value =
  {
    .m_pad1                              = 47,
    .m_v1.Type                           = SAHPI_STATUS_COND_TYPE_SENSOR,
    .m_v1.Entity.Entry[0].EntityType     = SAHPI_ENT_SYSTEM_BOARD,
    .m_v1.Entity.Entry[0].EntityLocation = 1,
    .m_v1.Entity.Entry[1].EntityType     = SAHPI_ENT_POWER_UNIT,
    .m_v1.Entity.Entry[1].EntityLocation = 2,
    .m_v1.Entity.Entry[2].EntityType     = SAHPI_ENT_ROOT,
    .m_v1.Entity.Entry[2].EntityLocation = 0,
    .m_v1.DomainId                       = SAHPI_UNSPECIFIED_DOMAIN_ID,
    .m_v1.ResourceId                     = 1,
    .m_v1.SensorNum                      = 1,
    .m_v1.EventState                     = SAHPI_ES_LOWER_MINOR,
    .m_v1.Name.Length                    = 9,
    .m_v1.Name.Value                     = "Next text",
    .m_v1.Mid                            = 3,
    .m_v1.Data.DataType                  = SAHPI_TL_TYPE_TEXT,
    .m_v1.Data.Language                  = SAHPI_LANG_ENGLISH,
    .m_v1.Data.DataLength                = 3,
    .m_v1.Data.Data                      = "AB",
    .m_pad2                  = 48,
    .m_v2.Type                           = SAHPI_STATUS_COND_TYPE_RESOURCE,
    .m_v2.Entity.Entry[0].EntityType     = SAHPI_ENT_SUB_CHASSIS,
    .m_v2.Entity.Entry[0].EntityLocation = 1,
    .m_v2.Entity.Entry[1].EntityType     = SAHPI_ENT_SYSTEM_BUS,
    .m_v2.Entity.Entry[1].EntityLocation = 2,
    .m_v2.Entity.Entry[2].EntityType     = SAHPI_ENT_COOLING_DEVICE,
    .m_v2.Entity.Entry[2].EntityLocation = 3,
    .m_v2.Entity.Entry[3].EntityType     = SAHPI_ENT_ROOT,
    .m_v2.Entity.Entry[3].EntityLocation = 0,
    .m_v2.DomainId                       = SAHPI_UNSPECIFIED_DOMAIN_ID,
    .m_v2.ResourceId                     = 2,
    .m_v2.SensorNum                      = 4,
    .m_v2.EventState                     = SAHPI_ES_LOWER_CRIT,
    .m_v2.Name.Length                    = 9,
    .m_v2.Name.Value                     = "Next text",
    .m_v2.Mid                            = 3,
    .m_v2.Data.DataType                  = SAHPI_TL_TYPE_TEXT,
    .m_v2.Data.Language                  = SAHPI_LANG_ENGLISH,
    .m_v2.Data.DataLength                = 3,
    .m_v2.Data.Data                      = "AB",
    .m_v3.Type                           = SAHPI_STATUS_COND_TYPE_OEM,
    .m_v3.Entity.Entry[0].EntityType     = SAHPI_ENT_PROCESSOR,
    .m_v3.Entity.Entry[0].EntityLocation = 1,
    .m_v3.Entity.Entry[1].EntityType     = SAHPI_ENT_ROOT,
    .m_v3.Entity.Entry[1].EntityLocation = 0,
    .m_v3.DomainId                       = SAHPI_UNSPECIFIED_DOMAIN_ID,
    .m_v3.ResourceId                     = 3,
    .m_v3.SensorNum                      = 5,
    .m_v3.EventState                     = SAHPI_ES_UPPER_MAJOR,
    .m_v3.Name.Length                    = 9,
    .m_v3.Name.Value                     = "Next text",
    .m_v3.Mid                            = 3,
    .m_v3.Data.DataType                  = SAHPI_TL_TYPE_BINARY,
    .m_v3.Data.Language                  = SAHPI_LANG_ENGLISH,
    .m_v3.Data.DataLength                = 3,
    .m_v3.Data.Data                      = "AB",
    .m_pad3                  = 49
  };

  unsigned char *buffer = (unsigned char *)malloc(sizeof(value));
  cTest          result;

  unsigned int s1 = Marshal( &TestType, &value, buffer );
  unsigned int s2 = Demarshal( MarshalByteOrder(), &TestType, &result, buffer );

  if ( s1 != s2 )
       return 1;

  if ( value.m_pad1 != result.m_pad1 )
       return 1;

  if ( !cmp_condition( &value.m_v1, &result.m_v1 ) )
       return 1;

  if ( value.m_pad2 != result.m_pad2 )
       return 1;

  if ( !cmp_condition( &value.m_v2, &result.m_v2 ) )
       return 1;

  if ( !cmp_condition( &value.m_v3, &result.m_v3 ) )
       return 1;

  if ( value.m_pad3 != result.m_pad3 )
       return 1;

  return 0;
}
