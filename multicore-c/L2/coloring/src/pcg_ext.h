#ifndef __H_PCG_EXT
#define __H_PCG_EXT

	extern int N2;
	extern int NUmax, NLmax, NCOLORtot, NCOLORk, NU, NL;

	extern double EPSICCG;
	extern double *D, *DELPHI, *BFORCE;
	extern double **AL, **AU;

	extern int *INL, *INU, *COLORindex;
	extern int *OLDtoNEW, *NEWtoOLD;

	extern int **IAL, **IAU;

#endif /* __H_PCG_EXT */
