/*
 * POI_GEN
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "struct_ext.h"
#include "pcg_ext.h"
#include "mc.h"
#include "cm.h"
#include "rcm.h"
#include "cmrcm.h"
#include "poi_gen.h"

extern int
POI_GEN(FILE *fp21)
{
	int nn, nnp;
	int icN1, icN2, icN3, icN4, icN5, icN6;
	int i, icel, icou, icouG, k;

	nn  = ICELTOT;

	NU = 6;
	NL = 6;

	INL = (int *)calloc(nn, sizeof(int));
	if(INL == NULL) {
		fprintf(stderr, "Error: %s\n", strerror(errno));
		return -1;
	}
	INU = (int *)calloc(nn, sizeof(int));
	if(INU == NULL) {
		fprintf(stderr, "Error: %s\n", strerror(errno));
		return -1;
	}
	IAL = (int **)calloc(nn, sizeof(int *));
	if(IAL == NULL) {
		fprintf(stderr, "Error: %s\n", strerror(errno));
		return -1;
	}
	for(i=0; i<nn; i++) {
		IAL[i] = (int *)calloc(NL, sizeof(int));
		if(IAL[i] == NULL) {
			fprintf(stderr, "Error: %s\n", strerror(errno));
			return -1;
		}
	}
	IAU = (int **)calloc(nn, sizeof(int *));
	if(IAU == NULL) {
		fprintf(stderr, "%s\n", strerror(errno));
		return -1;
	}
	for(i=0; i<nn; i++) {
		IAU[i] = (int *)calloc(NU, sizeof(int));
		if(IAU[i] == NULL) {
			fprintf(stderr, "Error: %s\n", strerror(errno));
			return -1;
		}
	}

/****************
 * CONNECTIVITY *
 ****************/
	for(icel=0; icel<ICELTOT; icel++) {
		icN1 = NEIBcell[icel][0];
		icN2 = NEIBcell[icel][1];
		icN3 = NEIBcell[icel][2];
		icN4 = NEIBcell[icel][3];
		icN5 = NEIBcell[icel][4];
		icN6 = NEIBcell[icel][5];

		icouG = 0;

		if(icN5 != 0) {
			icou = INL[icel] + 1;
			IAL[icel][icou-1] = icN5;
			INL[icel]         = icou;
		}

		if(icN3 != 0) {
			icou = INL[icel] + 1;
			IAL[icel][icou-1] = icN3;
			INL[icel]         = icou;
		}

		if(icN1 != 0) {
			icou = INL[icel] + 1;
			IAL[icel][icou-1] = icN1;
			INL[icel]         = icou;
		}

		if(icN2 != 0) {
			icou = INU[icel] + 1;
			IAU[icel][icou-1] = icN2;
			INU[icel]         = icou;
		}

		if(icN4 != 0) {
			icou = INU[icel] + 1;
			IAU[icel][icou-1] = icN4;
			INU[icel]         = icou;
		}

		if(icN6 != 0) {
			icou = INU[icel] + 1;
			IAU[icel][icou-1] = icN6;
			INU[icel]         = icou;
		}
	}

/************************
 * INITIAL connectivity *
 ************************/
	fprintf(fp21, "### INITIAL connectivity\n");
	for(i=0; i<ICELTOT; i++) {
		fprintf(fp21, "I=%5d     INL(i)=%5d     INU(i)=%5d\n", i+1, INL[i], INU[i]);
		fprintf(fp21, "  IAL:");
		for(k=0; k<INL[i]; k++) {
			fprintf(fp21, "%5d", IAL[i][k]);
			if((k+1)%6 == 0) fprintf(fp21, "\n");
		}
		if(k==0 || k%6) fprintf(fp21, "\n");
		fprintf(fp21, "  IAU:");
		for(k=0; k<INU[i]; k++) {
			fprintf(fp21, "%5d", IAU[i][k]);
			if((k+1)%6 == 0) fprintf(fp21, "\n");
		}
		if(k==0 || k%6) fprintf(fp21, "\n");
	}

/*****************
 * MULTICOLORING *
 *****************/
N111:
	fprintf(stderr, "\n\nYou have%8d elements\n", ICELTOT);
	fprintf(stderr, "How many colors do you need ?\n");
	fprintf(stderr, "  #COLOR must be more than 2 and\n");
	fprintf(stderr, "  #COLOR must not be more than%8d\n", ICELTOT);
	fprintf(stderr, "  if #COLOR= 0 then CM  ordering\n");
	fprintf(stderr, "  if #COLOR=-1 then RCM ordering\n");
	fprintf(stderr, "  if #COLOR<-1 then CMRCM ordering\n");
	fprintf(stderr, "=>\n");
	fscanf(stdin, "%d", &NCOLORtot);
	if(NCOLORtot == 1 && NCOLORtot > ICELTOT) goto N111;

	OLDtoNEW = (int *)calloc(ICELTOT, sizeof(int));
	if(OLDtoNEW == NULL) {
		fprintf(stderr, "Error: %s\n", strerror(errno));
		return -1;
	}
	NEWtoOLD = (int *)calloc(ICELTOT, sizeof(int));
	if(NEWtoOLD == NULL) {
		fprintf(stderr, "Error: %s\n", strerror(errno));
		return -1;
	}
	COLORindex = (int *)calloc(ICELTOT+1, sizeof(int));
	if(COLORindex == NULL) {
		fprintf(stderr, "Error: %s\n", strerror(errno));
		return -1;
	}

	if(NCOLORtot > 0) {
		MC(ICELTOT, NL, NU, INL, IAL, INU, IAU,
				&NCOLORtot, COLORindex, NEWtoOLD, OLDtoNEW);
	} else if(NCOLORtot == 0) {
		CM(ICELTOT, NL, NU, INL, IAL, INU, IAU,
				&NCOLORtot, COLORindex, NEWtoOLD, OLDtoNEW);
	} else if(NCOLORtot ==-1) {
		RCM(ICELTOT, NL, NU, INL, IAL, INU, IAU,
				&NCOLORtot, COLORindex, NEWtoOLD, OLDtoNEW);
	} else if(NCOLORtot <-1) {
		CMRCM(ICELTOT, NL, NU, INL, IAL, INU, IAU,
				&NCOLORtot, COLORindex, NEWtoOLD, OLDtoNEW);
	}

	fprintf(stderr, "\n# TOTAL COLOR number%8d\n", NCOLORtot);

	return 0;
}
