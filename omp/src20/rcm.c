/*
 * RCM
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "rcm.h"

extern int
RCM(int N, int NL, int NU, int *INL, int **IAL, int *INU, int **IAU,
	int *NCOLORtot, int *COLORindex, int *NEWtoOLD, int *OLDtoNEW)
{
	int **IW;
    int *INLw, *INUw;
    int **IALw, **IAUw;
	int KC, KCT, KCT0, KMIN, KMAX;
	int JC, JN;
	int II;
	int i, j, k, ic;
	int INmin, NODmin;
	int icon, icol, icouG, icou, icou0, in, inC, jL, jU;

/*********
 * INIT. *
 *********/
	IW = (int **)calloc(2, sizeof(int *));
	if(IW == NULL) {
		fprintf(stderr, "Error: %s\n", strerror(errno));
		return -1;
	}
	for(i=0; i<2; i++) {
		IW[i] = (int *)calloc(N, sizeof(int));
		if(IW[i] == NULL) {
			fprintf(stderr, "Error: %s\n", strerror(errno));
			return -1;
		}
	}

	INmin  = N;
	NODmin = 0;

	for(i=1; i<=N; i++) {
		icon = 0;
		for(k=1; k<=INL[i-1]; k++) {
			icon++;
		}
		for(k=1; k<=INU[i-1]; k++) {
			icon++;
		}

		if(icon < INmin) {
			INmin  = icon;
			NODmin = i;
		}
	}


	if(NODmin == 0) NODmin = 0;


	OLDtoNEW[NODmin-1] = 1;
	NEWtoOLD[0]      = NODmin;

	IW[1][NODmin-1] = 1;

	icol = 1;

/************
 * COLORING *
 ************/
	icouG = 1;
	for(icol=2; icol<=N; icol++) {
		icou  = 0;
		icou0 = 0;
		for(i=1; i<=N; i++) {
			if(IW[1][i-1] == icol-1) {
				for(k=1; k<=INL[i-1]; k++) {
					in = IAL[i-1][k-1];
					if(IW[1][in-1] == 0) {
						IW[1][in-1] = -icol;
						icou++;
						IW[0][icou-1] = in;
					}
				}
				for(k=1; k<=INU[i-1]; k++) {
					in = IAU[i-1][k-1];
					if(IW[1][in-1] == 0) {
						IW[1][in-1] = -icol ;
						icou++;
						IW[0][icou-1] = in;
					}
				}
			}
		}

		if(icou == 0) {
			for(i=1; i<=N; i++) {
				if(IW[1][i-1] == 0) {
					icou++;
					IW[1][i-1] = -icol;
					IW[0][icou-1] = i ;
					goto N850;
				}
			}
		}

N850:;

		for(ic=1; ic<=icou; ic++) {
			inC = IW[0][ic-1];
			if(IW[1][inC-1] != 0) {
				for(k=1; k<=INL[inC-1]; k++) {
					in = IAL[inC-1][k-1];
					if(IW[1][in-1] <= 0) {
						IW[1][in-1] = 0;
					}
				}
				for(k=1; k<=INU[inC-1]; k++) {
					in = IAU[inC-1][k-1];
					if(IW[1][in-1] <= 0) {
						IW[1][in-1] = 0;
					}
				}
			}
		}

		for(ic=1; ic<=icou; ic++) {
			inC = IW[0][ic-1];
			if(IW[1][inC-1] != 0) {
				icouG++;
				IW[1][inC-1] = icol;
			}
		}

		if(icouG == N) break;
	}

/******************/
/* FINAL COLORING */
/******************/

	*NCOLORtot = icol;

	fprintf(stderr, "NCOLORtot=%d\n", icol);

	icouG = 0;
	for(ic=(*NCOLORtot); ic>0; ic--) {
		icou = 0;
		for(i=N; i>0; i--) {
			if(IW[1][i-1] == ic) {
				icou++;
				icouG++;
				NEWtoOLD[icouG-1] = i;
				OLDtoNEW[i-1    ] = icouG;
			}
		}
		COLORindex[*NCOLORtot-ic+1] = icou;
	}

	COLORindex[0] = 0;
	for(ic=1; ic<=(*NCOLORtot); ic++) {
		COLORindex[ic] = COLORindex[ic-1] + COLORindex[ic];
	}


/*******************/
/* MATRIX transfer */
/*******************/
    	INLw = (int *)calloc(N, sizeof(int));
    	if(INLw == NULL) {
 		fprintf(stderr, "Error: %s\n", strerror(errno));
		return -1;
    	}

	INUw = (int *)calloc(N, sizeof(int));
	if(INUw == NULL) {
		fprintf(stderr, "Error: %s\n", strerror(errno));
		return -1;
	}

	IALw = (int **)calloc(N, sizeof(int *));
	if(IALw == NULL) {
		fprintf(stderr, "Error: %s\n", strerror(errno));
		return -1;
	}
	for(i=0; i<N; i++) {
		IALw[i] = (int *)calloc(NL, sizeof(int));
		if(IALw[i] == NULL) {
			fprintf(stderr, "Error: %s\n", strerror(errno));
			return -1;
		}
	}

	IAUw = (int **)calloc(N, sizeof(int *));
	if(IAUw == NULL) {
		fprintf(stderr, "Error: %s\n", strerror(errno));
		return -1;
	}
	for(i=0; i<N; i++) {
		IAUw[i] = (int *)calloc(NU, sizeof(int));
		if(IAUw[i] == NULL) {
			fprintf(stderr, "Error: %s\n", strerror(errno));
			return -1;
		}
	}

	for(i=1; i<=NL; i++) {
		for(j=1; j<=N; j++) {
			IW[0][j-1] = IAL[NEWtoOLD[j-1]-1][i-1];
		}
		for(j=1; j<=N; j++) {
			IAL[j-1][i-1] = IW[0][j-1];
		}
	}

	for(i=1; i<=NU; i++) {
		for(j=1; j<=N; j++) {
			IW[0][j-1] = IAU[NEWtoOLD[j-1]-1][i-1];
		}
		for(j=1; j<=N; j++) {
			IAU[j-1][i-1] = IW[0][j-1];
		}
	}

	for(i=1; i<=N; i++) {
		IW[0][i-1] = INL[NEWtoOLD[i-1]-1];
	}

	for(i=1; i<=N; i++) {
		INLw[i-1] = IW[0][i-1];
	}

	for(i=1; i<=N; i++) {
		IW[0][i-1] = INU[NEWtoOLD[i-1]-1];
	}

	for(i=1; i<=N; i++) {
		INUw[i-1] = IW[0][i-1];
	}

	for(j=1; j<=N; j++) {
		for(i=1; i<=NL; i++) {
			if(IAL[j-1][i-1] == 0) {
				IALw[j-1][i-1] = 0;
			} else {
				IALw[j-1][i-1] = OLDtoNEW[IAL[j-1][i-1]-1];
			}
		}
	}

	for(j=1; j<=N; j++) {
		for(i=1; i<=NU; i++) {
			if(IAU[j-1][i-1] == 0) {
				IAUw[j-1][i-1] = 0;
			} else {
				IAUw[j-1][i-1] = OLDtoNEW[IAU[j-1][i-1]-1];
			}
		}
	}

	for(j=0; j<N; j++) {
		INL[j] = 0;
		INU[j] = 0;
		for(i=0; i<NL; i++) {
			IAL[j][i] = 0;
		}
		for(i=0; i<NU; i++) {
			IAU[j][i] = 0;
		}
	}

	for(j=1; j<=N; j++) {
		jL = 0;
		jU = 0;
		for(i=1; i<=INLw[j-1]; i++) {
			if(IALw[j-1][i-1] > j ) {
				jU++;
				IAU[j-1][jU-1] = IALw[j-1][i-1];
			} else {
				jL++;
				IAL[j-1][jL-1] = IALw[j-1][i-1];
			}
		}

		for(i=1; i<=INUw[j-1]; i++) {
			if(IAUw[j-1][i-1] > j ) {
				jU++;
				IAU[j-1][jU-1] = IAUw[j-1][i-1];
			} else {
				jL++;
				IAL[j-1][jL-1] = IAUw[j-1][i-1];
			}
		}

		INL[j-1] = jL;
		INU[j-1] = jU;
	}


	free(IW);
	free(INLw);
	free(INUw);
	free(IALw);
	free(IAUw);

	return 0;
}
