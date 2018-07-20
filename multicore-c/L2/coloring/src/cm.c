/*
 * CM
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "rcm.h"

extern int
CM(int N, int NL, int NU, int *INL, int **IAL, int *INU, int **IAU,
	int *NCOLORtot, int *COLORindex, int *NEWtoOLD, int *OLDtoNEW)
{
	int **IW;
	int *INLw, *INUw;
	int **IALw, **IAUw;
	int KC, KCT, KCT0, KMIN, KMAX;
	int JC, JN;
	int II;
	int i, j, k, ic, jL, jU;
	int INmin, NODmin;
	int icon, icol, icouG, icou, icou0, in, inC;

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
	}

	INmin  = N;
	NODmin = -1;

	for(i=0; i<N; i++) {
		icon = 0;
		for(k=0; k<INL[i]; k++) {
			icon++;
		}
		for(k=0; k<INU[i]; k++) {
			icon++;
		}

		if(icon < INmin) {
			INmin  = icon;
			NODmin = i;
		}
	}


	if(NODmin == -1) NODmin = 0;

	IW[1][NODmin] = 1;

	NEWtoOLD[0]      = NODmin+1;
	OLDtoNEW[NODmin] = 1;

	icol = 0;


/************
 * COLORING *
 ************/
	icouG = 1;
	for(icol=1; icol<N; icol++) {
		icou  = 0;
		icou0 = 0;
		for(i=0; i<N; i++) {
			if(IW[1][i] == icol) {
				for(k=0; k<INL[i]; k++) {
					in = IAL[i][k];
					if(IW[1][in-1] == 0) {
						IW[1][in-1] = -(icol + 1);
						icou++;
						IW[0][icou-1] = in;
					}
				}
				for(k=0; k<INU[i]; k++) {
					in = IAU[i][k];
					if(IW[1][in-1] == 0) {
						IW[1][in-1] = -(icol + 1);
						icou++;
						IW[0][icou-1] = in;
					}
				}
			}
		}

		if(icou == 0) {
			for(i=0; i<N; i++) {
				if(IW[1][i] == 0) {
					icou++;
					IW[1][i     ] = -(icol + 1);
					IW[0][icou-1] = i + 1;
					break;
				}
			}
		}

		for(ic=0; ic<icou; ic++) {
			inC = IW[0][ic];
			if(IW[1][inC-1] != 0) {
				for(k=0; k<INL[inC-1]; k++) {
					in = IAL[inC-1][k];
					if(IW[1][in-1] <= 0) {
						IW[1][in-1] = 0;
					}
				}
				for(k=0; k<INU[inC-1]; k++) {
					in = IAU[inC-1][k];
					if(IW[1][in-1] <= 0) {
						IW[1][in-1] = 0;
					}
				}
			}
		}

		for(ic=0; ic<icou; ic++) {
			inC = IW[0][ic];
			if(IW[1][inC-1] != 0) {
				icouG++;
				IW[1][inC-1] = icol + 1;
			}
		}

		if(icouG == N) break;
	}

/******************/
/* FINAL COLORING */
/******************/
	*NCOLORtot = icol + 1;
	icouG = 0;
	for(ic=1; ic<=(*NCOLORtot); ic++) {
		icou = 0;
		for(i=0; i<N; i++) {
			if(IW[1][i] == ic) {
				NEWtoOLD[icouG] = 1+i;
				OLDtoNEW[i    ] = 1+icouG;
				icou++;
				icouG++;
			}
		}
		COLORindex[ic] = icou;
	}

	COLORindex[0] = 0;
	for(ic=1; ic<=(*NCOLORtot); ic++) {
		COLORindex[ic] = COLORindex[ic-1] + COLORindex[ic];
	}

/*******************
 * MATRIX transfer *
 *******************/
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
		IAUw[i] = (int *)calloc(NU, sizeof(int *));
		if(IAUw[i] == NULL) {
			fprintf(stderr, "Error: %s\n", strerror(errno));
			return -1;
		}
	}

	for(j=0; j<NL; j++) {
		for(i=0; i<N; i++) {
			IW[1][i] = IAL[NEWtoOLD[i]-1][j];
		}
		for(i=0; i<N; i++) {
			IAL[i][j] = IW[1][i];
		}
	}

	for(j=0; j<NU; j++) {
		for(i=0; i<N; i++) {
			IW[1][i] = IAU[NEWtoOLD[i]-1][j];
		}
		for(i=0; i<N; i++) {
			IAU[i][j] = IW[1][i];
		}
	}

	for(i=0; i<N; i++) {
		IW[1][i] = INL[NEWtoOLD[i]-1];
	}

	for(i=0; i<N; i++) {
		INLw[i] = IW[1][i];
	}

	for(i=0; i<N; i++) {
		IW[1][i] = INU[NEWtoOLD[i]-1];
	}

	for(i=0; i<N; i++) {
		INUw[i] = IW[1][i];
	}

	for(j=0; j<NL; j++) {
		for(i=0; i<N; i++) {
			if(IAL[i][j] == 0) {
				IALw[i][j] = 0;
			} else {
				IALw[i][j] = OLDtoNEW[IAL[i][j]-1];
			}
		}
	}

	for(j=0; j<NU; j++) {
		for(i=0; i<N; i++) {
			if(IAU[i][j] == 0) {
				IAUw[i][j] = 0;
			} else {
				IAUw[i][j] = OLDtoNEW[IAU[i][j]-1];
			}
		}
	}

	memset(INL, 0, sizeof(int)*N);
	memset(INU, 0, sizeof(int)*N);
	for(i=0; i<N; i++) {
		memset(IAL[i], 0, sizeof(int)*NL);
	}
	for(i=0; i<N; i++) {
		memset(IAU[i], 0, sizeof(int)*NU);
	}

	for(i=0; i<N; i++) {
		jL = 0;
		jU = 0;

		for(j=0; j<INLw[i]; j++) {
			if(IALw[i][j] > i+1) {
				IAU[i][jU] = IALw[i][j];
				jU++;
			} else {
				IAL[i][jL] = IALw[i][j];
				jL++;
			}
		}

		for(j=0; j<INUw[i]; j++) {
			if(IAUw[i][j] > i+1) {
				IAU[i][jU] = IAUw[i][j];
				jU++;
			} else {
				IAL[i][jL] = IAUw[i][j];
				jL++;
			}
		}

		INL[i] = jL;
		INU[i] = jU;
	}

	free(IW);
	free(INLw);
	free(INUw);
	free(IALw);
	free(IAUw);

	return 0;
}
