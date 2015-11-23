#include <gtk/gtk.h>
#include <stdio.h>
#include "sim.h"
#include "simglb.h"
#include "prototypes.h"


/*
 *	This is the main handler for all IN op-codes,
 *	called by the simulator. It calls the input
 *	function for port adr.
 */

BYTE io_in(BYTE adr)
{
  return((BYTE)IOPort_IN(adr));
}


/*
 *	This is the main handler for all OUT op-codes,
 *	called by the simulator. It calls the output
 *	function for port adr.
 */

void io_out(BYTE adr, BYTE data)
{
  IOPort_OUT(adr, data);
}


/*
 *	Stub to keep call from simint.c happy.
 */

void exit_io()
{
}
