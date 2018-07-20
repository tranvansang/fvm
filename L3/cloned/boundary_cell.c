#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "struct_ext.h"
#include "boundary_cell.h"
#include "allocate.h"

extern int
BOUNDARY_CELL(void)
{
	int IFACTOT;
	int icou, icel, i, j, k;

	IFACTOT = NX * NY;
	ZmaxCELtot = IFACTOT;

	ZmaxCEL = (int *)allocate_vector(sizeof(int),ZmaxCELtot);

	icou = 0;
	k    = NZ - 1;
	for(j=0; j<NY; j++) {
		for(i=0; i<NX; i++) {
			icel = k * IFACTOT + j * NX + i + 1;
			ZmaxCEL[icou] = icel;
			icou++;
		}
	}

	return 0;
}
