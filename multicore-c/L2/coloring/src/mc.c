/*
 * Multicolor Ordering Method
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "mc.h"

int
MC(int N, int NL, int NU, int *INL, int **IAL, int *INU, int **IAU,
	int *NCOLORtot, int *COLORindex, int *NEWtoOLD, int *OLDtoNEW)
{
	int NCOLORk;
	int *IW, *INLw, *INUw;
	int **IALw, **IAUw;
	int INmin, NODmin, ITEMcou;
	int i, j, k, icon, icou, icouK, icoug, icol, ic, ik, jL, jU;

/*********
 * INIT. *
 *********/
	IW = (int *)calloc(N, sizeof(int));
	if(IW == NULL) {
		fprintf(stderr, "Error: %s\n", strerror(errno));
		return -1;
	}

	NCOLORk = *NCOLORtot;

	for(i=0; i<N; i++) {
		NEWtoOLD[i] = i+1;
		OLDtoNEW[i] = i+1;
	}

	INmin = N;
	NODmin = -1;
	for(i=0; i<N; i++) {
		icon = INL[i] + INU[i];
		if (icon < INmin) {
			INmin = icon;
			NODmin = i;
		}
	}

	OLDtoNEW[NODmin] = 1;
	NEWtoOLD[0     ] = 1+NODmin;

	memset(IW, 0, sizeof(int)*N);
	IW[NODmin] = 1;

	ITEMcou = N / NCOLORk;

/*****************
 * MULTIcoloring *
 *****************/
	icou = 1;
	icouK = 1;
	for(icol=0; icol<N; icol++) {
		NCOLORk = icol + 1;
		for(i=0; i<N; i++) {
			if(IW[i] <= 0) IW[i] = 0;
		}
		for(i=0; i<N; i++) {
/*
 * already COLORED
 */
			if(IW[i] == icol+1) {
				for(k=0; k<INL[i]; k++) {
					ik = IAL[i][k];
					if(IW[ik-1] <= 0) IW[ik-1] = -1;
				}
				for(k=0; k<INU[i]; k++) {
					ik = IAU[i][k];
					if(IW[ik-1] <= 0) IW[ik-1] = -1;
				}
			}

/*
 * not COLORED
 */
			if(IW[i] == 0) {
				icou++;
				icouK++;

				IW[i] = icol + 1;
				for(k=0; k<INL[i]; k++) {
					ik = IAL[i][k];
					if(IW[ik-1] <= 0) IW[ik-1] = -1;
				}
				for(k=0; k<INU[i]; k++) {
					ik = IAU[i][k];
					if(IW[ik-1] <= 0) IW[ik-1] = -1;
				}
			}
			if(icou == N) goto N200;
			if(icouK == ITEMcou) goto N100;
		}
N100:
		icouK = 0;
	}
N200:

/******************
 * Final Coloring *
 ******************/
	*NCOLORtot = NCOLORk;
	memset(COLORindex, 0, sizeof(int)*(N+1));
	icoug = 0;
	for(ic=1; ic<=(*NCOLORtot); ic++) {
		icou = 0;
		for(i=0; i<N; i++) {
			if(IW[i] == ic) {
				NEWtoOLD[icoug] = 1+i;
				OLDtoNEW[i]     = 1+icoug;
				icou++;
				icoug++;
			}
		}
		COLORindex[ic] = icou;
	}

	for(ic=1; ic<=(*NCOLORtot); ic++) {
		COLORindex[ic] += COLORindex[ic-1];
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
			IW[i] = IAL[NEWtoOLD[i]-1][j];
		}
		for(i=0; i<N; i++) {
			IAL[i][j] = IW[i];
		}
	}

	for(j=0; j<NU; j++) {
		for(i=0; i<N; i++) {
			IW[i] = IAU[NEWtoOLD[i]-1][j];
		}
		for(i=0; i<N; i++) {
			IAU[i][j] = IW[i];
		}
	}

	for(i=0; i<N; i++) {
		IW[i] = INL[NEWtoOLD[i]-1];
	}

	for(i=0; i<N; i++) {
		INLw[i] = IW[i];
	}

	for(i=0; i<N; i++) {
		IW[i] = INU[NEWtoOLD[i]-1];
	}

	for(i=0; i<N; i++) {
		INUw[i] = IW[i];
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
