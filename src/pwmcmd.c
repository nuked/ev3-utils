/*
 *	pwmcmd.c -- PWM command test for EV3 (via /dev/ev3dev_pwm)
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
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>

#include "support.h"


#ifndef EXIT_SUCCESS
#define EXIT_SUCCESS (0)
#endif
#ifndef EXIT_FAILURE
#define EXIT_FAILURE (1)
#endif

/*}}}*/

/*{{{  defaults*/

#define DEFAULT_DEVICE "/dev/ev3dev_pwm"

/*}}}*/
/*{{{  globals*/
char *progname = NULL;

/*}}}*/
/*{{{  locals*/
#define MAX_BYTECODE_LEN (16)

static int verbose = 0;
static char bytecode[MAX_BYTECODE_LEN];
static int bytecode_len = 0;

/*}}}*/


/*{{{  static void usage (FILE *stream)*/
/*
 *	prints usage info.
 */
static void usage (FILE *stream)
{
	fprintf (stream, "Usage: %s [options] <cmd> [cmd ...]\n", progname);
	fprintf (stream, "where options are:\n");
	fprintf (stream, "  -h | --help            show this help\n");
	fprintf (stream, "  -d | --device <dev>    use named device (default is %s)\n", DEFAULT_DEVICE);
	fprintf (stream, "  -v | --verbose         be more verbose (incremental)\n");
	fprintf (stream, "and commands are:\n");
	fprintf (stream, "  0x..                   hexadecimal bytecode byte\n");
	fprintf (stream, "  Wnnn                   wait nnn milliseconds\n");
	fprintf (stream, "  S                      send constructed buffer to device\n");

	return;
}
/*}}}*/
/*{{{  static int dump_bytecodes (int fd)*/
/*
 *	dumps the bytecode command buffer to the device.
 *	returns 0 on success (and buffer cleared), non-zero on failure.
 */
static int dump_bytecodes (int fd)
{
	int i;

	if (!bytecode_len) {
		return 0;
	}

	if (verbose) {
		prog_info ("writing %d bytes...", bytecode_len);
	}

	i = write (fd, bytecode, bytecode_len);
	if (i < 0) {
		prog_error ("error writing %d bytes to device: %s", bytecode_len, strerror (errno));
		return -1;
	} else if (i != bytecode_len) {
		prog_error ("tried to write %d bytes to device, but got back %d", bytecode_len, i);
		return -1;
	} else {
		/* must have been successful! */
		bytecode_len = 0;
	}
	return 0;
}
/*}}}*/


/*{{{  int main (int argc, char **argv)*/
/*
 *	start here.
 */
int main (int argc, char **argv)
{
	int i;
	char **walk;
	char *opt_device = DEFAULT_DEVICE;
	int dev_fd;
	int ret = EXIT_SUCCESS;

	/*{{{  sort progname out & other initialisation*/
	bytecode_len = 0;
	for (progname = (*argv + (strlen (*argv) - 1)); (progname > *argv) && (progname[-1] != '/'); progname--);

	/*}}}*/
	/*{{{  process command-line options until first command*/
	for (i=argc-1, walk=argv+1; i && *walk; i--, walk++) {
		if (((*walk)[0] == '-') && ((*walk)[1] != '-')) {
			/* short options */
			char *ch;

			for (ch = (*walk) + 1; *ch != '\0'; ch++) {
				switch (*ch) {
				case 'd':
					i--, walk++;
					if ((ch[1] != '\0') || !i || !*walk) {
						fprintf (stderr, "%s: option -%c requires argument.\n", progname, ch[-1]);
						exit (EXIT_FAILURE);
					}
					opt_device = *walk;
					break;
				case 'v':
					verbose++;
					break;
				case 'h':
					usage (stdout);
					exit (EXIT_FAILURE);
					break;
				default:
					fprintf (stderr, "%s: unknown option -%c, --help for usage.\n", progname, *ch);
					exit (EXIT_FAILURE);
					break;
				}
			}
		} else if (!strcmp (*walk, "--help")) {
			usage (stdout);
			exit (EXIT_FAILURE);
		} else if (!strcmp (*walk, "--device")) {
			i--, walk++;
			if (!i || !*walk) {
				fprintf (stderr, "%s: option %s requires argument.\n", progname, walk[-1]);
				exit (EXIT_FAILURE);
			}
			opt_device = *walk;
		} else if (!strcmp (*walk, "--verbose")) {
			verbose++;
		} else if ((*walk)[0] == '-') {
			/* unknown */
			fprintf (stderr, "%s: unknown option %s, --help for usage.\n", progname, *walk);
			exit (EXIT_FAILURE);
		} else {
			break;			/* for() */
		}
	}

	if (verbose) {
		prog_info ("using device [%s]", opt_device);
	}

	if (!i || !*walk) {
		fprintf (stderr, "%s: no commands given!\n", progname);
		exit (EXIT_SUCCESS);
	}

	/*}}}*/

	/*{{{  open PWM device*/
	dev_fd = open (opt_device, O_WRONLY);
	if (dev_fd < 0) {
		prog_error ("failed to open %s: %s", opt_device, strerror (errno));
		exit (EXIT_FAILURE);
	}

	/*}}}*/
	/*{{{  process commands*/
	for (; i && *walk && (bytecode_len < MAX_BYTECODE_LEN); i--, walk++) {
		if (!strncmp (*walk, "0x", 2)) {
			/*{{{  assume hex byte -- add to array*/
			unsigned int v;

			if (sscanf ((*walk) + 2, "%x", &v) != 1) {
				prog_error ("bad hex byte [%s]", *walk);
				ret = EXIT_FAILURE;
				goto out_err;
			} else if (v > 255) {
				prog_error ("hex value [%s] out of range (0-255)", *walk);
				ret = EXIT_FAILURE;
				goto out_err;
			}
			/* pop it in */
			bytecode[bytecode_len++] = (char)v;
			/*}}}*/
		} else if (**walk == 'W') {
			/*{{{  wait specified time in milliseconds*/
			int ms;

			if (sscanf ((*walk) + 1, "%d", &ms) != 1) {
				prog_error ("bad wait time [%s]", (*walk) + 1);
				ret = EXIT_FAILURE;
				goto out_err;
			} else if ((ms <= 0) || (ms > 1000000)) {
				prog_error ("wait time of %d milliseconds is daft", ms);
				ret = EXIT_FAILURE;
				goto out_err;
			}

			/* dump what we have into the device and wait */
			if (dump_bytecodes (dev_fd)) {
				ret = EXIT_FAILURE;
				goto out_err;
			}

			if (verbose) {
				prog_info ("sleeping for %d milliseconds...", ms);
			}
			if (usleep (ms * 1000)) {
				prog_warning ("short sleep?: %s", strerror (errno));
			}

			/*}}}*/
		} else if (**walk == 'S') {
			/*{{{  send buffer to device*/
			if (dump_bytecodes (dev_fd)) {
				ret = EXIT_FAILURE;
				goto out_err;
			}

			/*}}}*/
		}
	}
	/* if we have any left, run through */
	if (dump_bytecodes (dev_fd)) {
		ret = EXIT_FAILURE;
		goto out_err;
	}

	/*}}}*/

out_err:
	/*{{{  close PWM device*/
	close (dev_fd);
	dev_fd = -1;

	/*}}}*/

	return ret;
}
/*}}}*/

