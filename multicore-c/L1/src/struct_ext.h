#ifndef __H_STRUCT_EXT
#define __H_STRUCT_EXT

/*
 * METRICs & FLUX
 */
	extern int ICELTOT, ICELTOTp, N;
	extern int NX, NY, NZ, NXP1, NYP1, NZP1, IBNODTOT;
	extern int NXc, NYc, NZc;

	extern double DX, DY, DZ, XAREA, YAREA, ZAREA;
	extern double RDX, RDY, RDZ, RDX2, RDY2, RDZ2, R2DX, R2DY, R2DZ;
	extern double *VOLCEL, *VOLNOD, *RVC, *RVN;

	extern int **XYZ, **NEIBcell;

/*
 * BOUNDARYs
 */
	extern int ZmaxCELtot;
	extern int *BC_INDEX, *BC_NOD;
	extern int *ZmaxCEL;

/*
 * WORK
 */
	extern int **IWKX;
	extern double **FCV;

	extern int my_rank, PETOT, PEsmpTOT;

#endif /* __H_STRUCT_EXT */
