#ifndef __H_STRUCT
#define __H_STRUCT

/*
 * METRICs & FLUX
 */
	int ICELTOT, ICELTOTp, N;
	int NX, NY, NZ, NXP1, NYP1, NZP1, IBNODTOT;
	int NXc, NYc, NZc;

	double DX, DY, DZ, XAREA, YAREA, ZAREA;
	double RDX, RDY, RDZ, RDX2, RDY2, RDZ2, R2DX, R2DY, R2DZ;
	double *VOLCEL, *VOLNOD, *RVC, *RVN;

	int **XYZ, **NEIBcell;

/*
 * BOUNDARYs
 */
	int ZmaxCELtot;
	int *BC_INDEX, *BC_NOD;
	int *ZmaxCEL;

/*
 * WORK
 */
	int **IWKX;
	double **FCV;

/*
 * MPI
 */
	int my_rank, PETOT, PEsmpTOT;

#endif /* __H_STRUCT */
