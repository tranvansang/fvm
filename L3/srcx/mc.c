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

	for(i=1; i<=N; i++) {
		NEWtoOLD[i-1] = i;
		OLDtoNEW[i-1] = i;
	}

	INmin = N;
	NODmin = -1;
	for(i=1; i<=N; i++) {
		icon = INL[i-1] + INU[i-1];
		if (icon < INmin) {
			INmin = icon;
			NODmin = i;
		}
	}

	OLDtoNEW[NODmin-1] = 1;
	NEWtoOLD[0       ] = NODmin;

	memset(IW, 0, sizeof(int)*N);
	IW[0] = 1;

	ITEMcou = N / NCOLORk;

/*****************
 * MULTIcoloring *
 *****************/
	icou = 1;
	icouK = 1;
	for(icol=1; icol<=N; icol++) {
		NCOLORk = icol;
		for(i=1; i<=N; i++) {
			if(IW[i-1] <= 0) IW[i-1] = 0;
		}
		for(i=1; i<=N; i++) {
/*
 * already COLORED
 */
			if(IW[i-1] == icol) {
				for(k=1; k<=INL[i-1]; k++) {
					ik = IAL[i-1][k-1];
					if(IW[ik-1] <= 0) IW[ik-1] = -1;
				}
				for(k=1; k<=INU[i-1]; k++) {
					ik = IAU[i-1][k-1];
					if(IW[ik-1] <= 0) IW[ik-1] = -1;
				}
			}

/*
 * not COLORED
 */
			if(IW[i-1] == 0) {
				icou++;
				icouK++;

				IW[i-1] = icol;
				for(k=1; k<=INL[i-1]; k++) {
					ik = IAL[i-1][k-1];
					if(IW[ik-1] <= 0) IW[ik-1] = -1;
				}
				for(k=1; k<=INU[i-1]; k++) {
					ik = IAU[i-1][k-1];
					if(IW[ik-1] <= 0) IW[ik-1] = -1;
				}
			}
			if(icou == N) goto N200;
			if(icouK == ITEMcou) goto N100;
		}
N100:;
		icouK = 0;
	}
N200:;

/******************
 * Final Coloring *
 ******************/
	*NCOLORtot = NCOLORk;
	memset(COLORindex, 0, sizeof(int)*(N+1));
	icoug = 0;
	for(ic=1; ic<=(*NCOLORtot); ic++) {
		icou = 0;
		for(i=1; i<=N; i++) {
			if(IW[i-1] == ic) {
				icou++;
				icoug++;
				NEWtoOLD[icoug-1] = i;
				OLDtoNEW[i    -1] = icoug;
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

	for(j=1; j<=NL; j++) {
		for(i=1; i<=N; i++) {
			IW[i-1] = IAL[NEWtoOLD[i-1]-1][j-1];
		}
		for(i=1; i<=N; i++) {
			IAL[i-1][j-1] = IW[i-1];
		}
	}

	for(j=1; j<=NU; j++) {
		for(i=1; i<=N; i++) {
			IW[i-1] = IAU[NEWtoOLD[i-1]-1][j-1];
		}
		for(i=1; i<=N; i++) {
			IAU[i-1][j-1] = IW[i-1];
		}
	}

	for(i=1; i<=N; i++) {
		IW[i-1] = INL[NEWtoOLD[i-1]-1];
	}

	for(i=1; i<=N; i++) {
		INLw[i-1] = IW[i-1];
	}

	for(i=1; i<=N; i++) {
		IW[i-1] = INU[NEWtoOLD[i-1]-1];
	}

	for(i=1; i<=N; i++) {
		INUw[i-1] = IW[i-1];
	}

	for(j=1; j<=NL; j++) {
		for(i=1; i<=N; i++) {
			if(IAL[i-1][j-1] == 0) {
				IALw[i-1][j-1] = 0;
			} else {
				IALw[i-1][j-1] = OLDtoNEW[IAL[i-1][j-1]-1];
			}
		}
	}

	for(j=1; j<=NU; j++) {
		for(i=1; i<=N; i++) {
			if(IAU[i-1][j-1] == 0) {
				IAUw[i-1][j-1] = 0;
			} else {
				IAUw[i-1][j-1] = OLDtoNEW[IAU[i-1][j-1]-1];
			}
		}
	}

	memset(INL, 0, sizeof(int)*N);
	memset(INU, 0, sizeof(int)*N);

	for(i=0; i<N; i++) {
		for(j=0; j<NL; j++){
			IAL[i][j]=0;
		}
	}
	for(i=0; i<N; i++) {
		for(j=0; j<NU; j++){
			IAU[i][j]=0;
		}
	}

	for(i=1; i<=N; i++) {
		jL = 0;
		jU = 0;

		for(j=1; j<=INLw[i-1]; j++) {
			if(IALw[i-1][j-1] > i) {
				IAU[i-1][jU] = IALw[i-1][j-1];
				jU++;
			} else {
				IAL[i-1][jL] = IALw[i-1][j-1];
				jL++;
			}
		}

		for(j=1; j<=INUw[i-1]; j++) {
			if(IAUw[i-1][j-1] > i) {
				IAU[i-1][jU] = IAUw[i-1][j-1];
				jU++;
			} else {
				IAL[i-1][jL] = IAUw[i-1][j-1];
				jL++;
			}
		}

		INL[i-1] = jL;
		INU[i-1] = jU;
	}

	free(IW);
	free(INLw);
	free(INUw);
	free(IALw);
	free(IAUw);

	return 0;
}
