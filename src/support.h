/*
 *	support.h -- simplified version of my stock support.h
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

#ifndef __SUPPORT_H
#define __SUPPORT_H

/* programs must define this! */
extern char *progname;


void prog_fatal (const char *fmt, ...);
void prog_warning (const char *fmt, ...);
void prog_error (const char *fmt, ...);
void prog_info (const char *fmt, ...);


void *smalloc (const int size);
void sfree (void *ptr);

char *string_dup (const char *s);


#endif	/* !__SUPPORT_H */

