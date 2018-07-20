/*
 * POINTER_INIT
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "struct_ext.h"
#include "pcg_ext.h"
#include "pointer_init.h"
#include "allocate.h"

extern int
POINTER_INIT(void)
{

        int icel, ipe, i, j, k;

/*
 * INIT.
 */
        ICELTOT = NX * NY * NZ;

        NXP1 = NX + 1;
        NYP1 = NY + 1;
        NZP1 = NZ + 1;


	NEIBcell = (int **)allocate_matrix(sizeof(int),ICELTOT,6);
	XYZ      = (int **)allocate_matrix(sizeof(int),ICELTOT,3);



/*
 * INDIVIDUAL REGION
 */
        for(k=0; k<NZ; k++) {
                for(j=0; j<NY; j++) {
                        for(i=0; i<NX; i++) {
                                icel = k * NX * NY + j * NX + i;
                                NEIBcell[icel][0] = icel - 1       + 1;
                                NEIBcell[icel][1] = icel + 1       + 1;
                                NEIBcell[icel][2] = icel - NX      + 1;
                                NEIBcell[icel][3] = icel + NX      + 1;
                                NEIBcell[icel][4] = icel - NX * NY + 1;
                                NEIBcell[icel][5] = icel + NX * NY + 1;
                                if(i == 0)    NEIBcell[icel][0] = 0;
                                if(i == NX-1) NEIBcell[icel][1] = 0;
                                if(j == 0)    NEIBcell[icel][2] = 0;
                                if(j == NY-1) NEIBcell[icel][3] = 0;
                                if(k == 0)    NEIBcell[icel][4] = 0;
                                if(k == NZ-1) NEIBcell[icel][5] = 0;

                                XYZ[icel][0] = i + 1;
                                XYZ[icel][1] = j + 1;
                                XYZ[icel][2] = k + 1;
                        }
                }
        }


	if(DX <= 0.0) {
		DX = 1.0 / (double)NX;
		DY = 1.0 / (double)NY;
		DZ = 1.0 / (double)NZ;
	}

	NXP1 = NX + 1;
	NYP1 = NY + 1;
	NZP1 = NZ + 1;

	IBNODTOT = NXP1 * NYP1;
	N        = NXP1 * NYP1 * NZP1;

	return 0;
}
