#ifndef __H_PCG
#define __H_PCG

	static int N2 = 256;
	int NUmax, NLmax, NCOLORtot, NCOLORk, NU, NL;

	double EPSICCG;
	double *D, *DELPHI, *BFORCE;
	double **AL, **AU;

	int *INL, *INU, *COLORindex;
	int *OLDtoNEW, *NEWtoOLD;

	int **IAL, **IAU;

#endif /* __H_PCG */
