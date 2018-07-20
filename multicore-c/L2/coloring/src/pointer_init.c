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

extern int
POINTER_INIT(void)
{
	int icel, ipe, i, j, k, ii;

        fprintf(stderr, "input NX NY NZ=>\n");
        fscanf(stdin, "%d%d%d", &NX, &NY, &NZ);
        fprintf(stderr, "NX=%d NY=%d NZ=%d\n", NX, NY, NZ);



/*
 * INIT.
 */
        ICELTOT = NX * NY * NZ;
        fprintf(stderr, "ICELTOT=%d\n", ICELTOT);

        NXP1 = NX + 1;
        NYP1 = NY + 1;
        NZP1 = NZ + 1;


	NEIBcell = (int **)malloc(sizeof(int *)*ICELTOT);
	if(NEIBcell == NULL) {
		fprintf(stderr, "Error: %s\n", strerror(errno));
		return -1;
	}
	for(i=0; i<ICELTOT; i++) {
		NEIBcell[i] = (int *)malloc(sizeof(int)*6);
		if(NEIBcell[i] == NULL) {
			fprintf(stderr, "Error: %s\n", strerror(errno));
			return -1;
		}
	}

	XYZ = (int **)malloc(sizeof(int *)*ICELTOT);
	if(XYZ == NULL) {
		fprintf(stderr, "Error: %s\n", strerror(errno));
		return -1;
	}
	for(i=0; i<ICELTOT; i++) {
		XYZ[i] = (int *)malloc(sizeof(int)*3);
		if(XYZ[i] == NULL) {
			fprintf(stderr, "Error: %s\n", strerror(errno));
			return -1;
		}
	}


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



	NXP1 = NX + 1;
	NYP1 = NY + 1;
	NZP1 = NZ + 1;

	IBNODTOT = NXP1 * NYP1;
	N        = NXP1 * NYP1 * NZP1;

	return 0;

}
