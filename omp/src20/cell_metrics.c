#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>

#include "struct_ext.h"
#include "pcg_ext.h"
#include "cell_metrics.h"
#include "allocate.h"

extern int
CELL_METRICS(void)
{
	double V0, RV0;
	int i;

/************
 * ALLOCATE *
 ************/
	VOLCEL = (double *)allocate_vector(sizeof(double),ICELTOT);
	RVC    = (double *)allocate_vector(sizeof(double),ICELTOT);

/*********************************
 * VOLUME, AREA, PROJECTION etc. *
 *********************************/
	XAREA = DY * DZ;
	YAREA = DZ * DX;
	ZAREA = DX * DY;

	RDX = 1.0 / DX;
	RDY = 1.0 / DY;
	RDZ = 1.0 / DZ;

	RDX2 = 1.0 / (pow(DX, 2.0));
	RDY2 = 1.0 / (pow(DY, 2.0));
	RDZ2 = 1.0 / (pow(DZ, 2.0));

	R2DX = 1.0 / (0.5 * DX);
	R2DY = 1.0 / (0.5 * DY);
	R2DZ = 1.0 / (0.5 * DZ);

	V0  = DX * DY * DZ;
	RV0 = 1.0 / V0;

	for(i=0; i<ICELTOT; i++) {
		VOLCEL[i] = V0;
		RVC[i]    = RV0;
	}

	return 0;
}
