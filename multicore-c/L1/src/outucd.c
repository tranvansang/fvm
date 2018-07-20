/*
 * OUTUCD
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "struct_ext.h"
#include "pcg_ext.h"
#include "outucd.h"

extern int
OUTUCD(void)
{
	char elem[4];
	int N0, N1;
	double XX, YY, ZZ;
	int iS, in1, in2, in3, in4, in5, in6, in7, in8;
	int icou, icol, i, j, k, ii;
	FILE *fp12;

	strcpy(elem, "hex");
	N0 = 0;
	N1 = 1;

	if((fp12 = fopen("test.inp", "w")) == NULL) {
		fprintf(stderr, "Error: %s\n", strerror(errno));
		return -1;
	}

        fprintf(fp12,"%8d%8d%8d%8d%8d\n",N,ICELTOT,N0,N1,N0);

	icou = 0;
	for(k=0; k<NZP1; k++) {
		for(j=0; j<NYP1; j++) {
			for(i=0; i<NXP1; i++) {
				icou++;
				XX = (double)i * DX;
				YY = (double)j * DY;
				ZZ = (double)k * DZ;
				fprintf(fp12, "%8d        %16.6E%16.6E%16.6E\n", icou, XX, YY, ZZ);
			}
		}
	}

	icou = 0;
	for(k=0; k<NZ; k++) {
		for(j=0; j<NY; j++) {
			for(i=0; i<NX; i++) {
				icou++;
				iS  = k * IBNODTOT + j * NXP1 + i + 1;
				in1 = iS;
				in2 = iS + 1;
				in3 = iS + 1 + NXP1;
				in4 = iS     + NXP1;
				in5 = in1 + IBNODTOT;
				in6 = in2 + IBNODTOT;
				in7 = in3 + IBNODTOT;
				in8 = in4 + IBNODTOT;
				fprintf(fp12, "%8d%3d %s  %8d%8d%8d%8d%8d%8d%8d%8d\n",
						icou, N1, elem, in1, in2, in3, in4, in5, in6, in7, in8);
			}
		}
	}

	fprintf(fp12, "%8d%8d\n", N1, N1);
	fprintf(fp12, "VAL,VAL\n");
	for(i=0; i<ICELTOT; i++) {
		fprintf(fp12, "%8d%16.6E\n", i+1, PHI[i]);
	}

	fclose(fp12);

	return 0;
}
