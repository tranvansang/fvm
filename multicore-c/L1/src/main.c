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
#include "solver_ICCG.h"
#include "solver_PCG.h"
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
	memset(PHI, 0.0, sizeof(double)*ICELTOT);

	ISET = 0;

	WK = (double *)malloc(sizeof(double)*ICELTOT);
	if(WK == NULL) {
		fprintf(stderr, "Error: %s\n", strerror(errno));
		goto error;
	}



        if(METHOD==1){
	  if(solve_ICCG(ICELTOT, NL, NU, indexL, itemL, indexU, itemU,
			D, BFORCE, PHI, AL, AU, EPSICCG, &ITR, &IER)) goto error;} else if(METHOD==3){
	  if(solve_PCG(ICELTOT, NL, NU, indexL, itemL, indexU, itemU,
			D, BFORCE, PHI, AL, AU, EPSICCG, &ITR, &IER)) goto error;}


	fprintf(stderr, "\nN= %10d\n", ICELTOT);

        if(OUTUCD()) goto error;

	return 0;

error:
	return -1;
}
