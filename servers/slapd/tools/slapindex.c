/* $OpenLDAP$ */
/* This work is part of OpenLDAP Software <http://www.openldap.org/>.
 *
 * Copyright 1998-2004 The OpenLDAP Foundation.
 * Portions Copyright 1998-2003 Kurt D. Zeilenga.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted only as authorized by the OpenLDAP
 * Public License.
 *
 * A copy of this license is available in file LICENSE in the
 * top-level directory of the distribution or, alternatively, at
 * <http://www.OpenLDAP.org/license.html>.
 */
/* ACKNOWLEDGEMENTS:
 * This work was initially developed by Kurt Zeilenga for inclusion
 * in OpenLDAP Software.
 */

#include "portable.h"

#include <stdio.h>

#include <ac/stdlib.h>

#include <ac/ctype.h>
#include <ac/string.h>
#include <ac/socket.h>
#include <ac/unistd.h>

#include "slapcommon.h"

int
main( int argc, char **argv )
{
	ID id;
	int rc = EXIT_SUCCESS;

	slap_tool_init( "slapindex", SLAPINDEX, argc, argv );

	if( !be->be_entry_open ||
		!be->be_entry_close ||
		!be->be_entry_first ||
		!be->be_entry_next  ||
		!be->be_entry_reindex )
	{
		fprintf( stderr, "%s: database doesn't support necessary operations.\n",
			progname );
		exit( EXIT_FAILURE );
	}

	if( be->be_entry_open( be, 0 ) != 0 ) {
		fprintf( stderr, "%s: could not open database.\n",
			progname );
		exit( EXIT_FAILURE );
	}

	for ( id = be->be_entry_first( be );
		id != NOID;
		id = be->be_entry_next( be ) )
	{
		int rtn;

		if( verbose ) {
			printf("indexing id=%08lx\n", (long) id );
		}

		rtn =  be->be_entry_reindex( be, id );

		if( rtn != LDAP_SUCCESS ) {
			rc = EXIT_FAILURE;
			if( continuemode ) continue;
			break;
		}
	}

	(void) be->be_entry_close( be );

	slap_tool_destroy();
	return( rc );
}
