/*
 *	support.c -- simplified version of my stock support.c
 *	Copyright (C) 2014 Fred Barnes <frmb@kent.ac.uk>
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 2 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program; if not, write to the Free Software
 *	Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
 */

/*{{{  includes, etc.*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#include "support.h"


/*}}}*/


/*{{{  void prog_fatal (const char *fmt, ...)*/
/*
 *	fatal error (exits).
 */
void prog_fatal (const char *fmt, ...)
{
	va_list ap;

	fprintf (stderr, "%s: fatal: ", progname);
	va_start (ap, fmt);
	vfprintf (stderr, fmt, ap);
	va_end (ap);
	fprintf (stderr, "\n");

	exit (EXIT_FAILURE);
}
/*}}}*/
/*{{{  void prog_warning (const char *fmt, ...)*/
/*
 *	warning.
 */
void prog_warning (const char *fmt, ...)
{
	va_list ap;

	fprintf (stderr, "%s: warning: ", progname);
	va_start (ap, fmt);
	vfprintf (stderr, fmt, ap);
	va_end (ap);
	fprintf (stderr, "\n");

	return;
}
/*}}}*/
/*{{{  void prog_error (const char *fmt, ...)*/
/*
 *	error.
 */
void prog_error (const char *fmt, ...)
{
	va_list ap;

	fprintf (stderr, "%s: error: ", progname);
	va_start (ap, fmt);
	vfprintf (stderr, fmt, ap);
	va_end (ap);
	fprintf (stderr, "\n");

	return;
}
/*}}}*/
/*{{{  void prog_info (const char *fmt, ...)*/
/*
 *	informational.
 */
void prog_info (const char *fmt, ...)
{
	va_list ap;

	fprintf (stderr, "%s: ", progname);
	va_start (ap, fmt);
	vfprintf (stderr, fmt, ap);
	va_end (ap);
	fprintf (stderr, "\n");

	return;
}
/*}}}*/


/*{{{  void *smalloc (const int size)*/
/*
 *	does memory allocation.
 */
void *smalloc (const int size)
{
	void *p;
	
	if (size <= 0) {
		prog_fatal ("impossible smalloc(%d)", size);
		return NULL;
	}
	p = malloc (size);
	if (!p) {
		prog_fatal ("out of memory (%d bytes)", size);
		return NULL;
	}
	memset (p, 0, size);

	return p;
}
/*}}}*/
/*{{{  void sfree (void *ptr)*/
/*
 *	does memory release.
 */
void sfree (void *ptr)
{
	if (!ptr) {
		prog_warning ("sfree(): NULL pointer!");
	} else {
		free (ptr);
	}
}
/*}}}*/

/*{{{  char *string_dup (const char *s)*/
/*
 *	duplicates a string.
 */
char *string_dup (const char *s)
{
	int sl = strlen (s);
	char *ns = (char *)smalloc (sl + 1);

	memcpy (ns, s, sl);
	ns[sl] = '\0';

	return ns;
}
/*}}}*/

