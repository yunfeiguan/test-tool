/*
 * Copyright (c) 2004 by FORCE Computers.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  This
 * file and program are licensed under a BSD style license.  See
 * the Copying file included with the OpenHPI distribution for
 * full licensing terms.
 *
 * Authors:
 *     Thomas Kanngieser <thomas.kanngieser@fci.com>
 */

#include "marshal.h"


int
main( int argc, char *argv[] )
{
  tInt16         value = 0x42aa;
  tInt16         result;
  unsigned char  buffer[256];

  unsigned int s1 = Marshal( &Marshal_Int16Type, &value, buffer );

  if ( s1 != sizeof( tInt16 ) )
       return 1;

  unsigned int s2 = Demarshal( MarshalByteOrder(), &Marshal_Int16Type, &result, buffer );

  if ( s2 != sizeof( tInt16 ) )
       return 1;

  if ( value != result )
       return 1;

  return 0;
}
