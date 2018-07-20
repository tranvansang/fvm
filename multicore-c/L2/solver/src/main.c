#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "struct.h"
#include "pcg.h"
#include "input.h"
#include "pointer_init.h"
#include "boundary_cell.h"
#include "cell_metrics.h"
#include "poi_gen.h"
#include "solver_ICCG_mc.h"
#include "outucd.h"

int
main()
{
	double *WK;
	int NPL, NPU;
	int ISET, ITR, IER;
	int icel, ic0, i;
	double xN, xL, xU;
	double Stime, Etime;

/*********
 * INIT. *
 *********/
	if(INPUT()) goto error;
	if(POINTER_INIT()) goto error;
	if(BOUNDARY_CELL()) goto error;
	if(CELL_METRICS()) goto error;
	if(POI_GEN()) goto error;


/***************
 * MAIN SOLVER *
 ***************/
	ISET = 0;

	WK = (double *)malloc(sizeof(double)*ICELTOT);
	if(WK == NULL) {
		fprintf(stderr, "Error: %s\n", strerror(errno));
		goto error;
	}


	if(solve_ICCG_mc(ICELTOT, NL, NU, indexL, itemL, indexU, itemU,
			 D, BFORCE, PHI, AL, AU, NCOLORtot, COLORindex,
			 EPSICCG, &ITR, &IER)) goto error;

	fprintf(stderr, "\nN= %10d\n", ICELTOT);

	for(ic0=0; ic0<ICELTOT; ic0++) {
		icel = NEWtoOLD[ic0];
		WK[icel-1] = PHI[ic0];
	}

	for(icel=0; icel<ICELTOT; icel++) {
		PHI[icel] = WK[icel];
	}

        if(OUTUCD()) goto error;

	return 0;

error:
	return -1;
}
