#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "struct.h"
#include "pcg.h"
#include "pointer_init.h"
#include "poi_gen.h"
#include "outucd.h"

int
main(int argc, char **argv)
{
	FILE *fp21;
	int i, ic, k;

	if(POINTER_INIT()) goto error;

	if((fp21 = fopen("color.log", "w")) == NULL) {
		fprintf(stderr, "Error: %s\n", strerror(errno));
		return -1;
	}
	if(POI_GEN(fp21)) goto error;

	if(OUTUCD()) goto error;

	fprintf(fp21, "\n\nCOLOR number%8d\n\n", NCOLORtot);
	for(ic=1; ic<=NCOLORtot; ic++) {
		for(i=COLORindex[ic-1]+1; i<=COLORindex[ic]; i++) {
			fprintf(fp21, "  #new%8d   #old%8d  color%8d\n", i, NEWtoOLD[i-1], ic);
		}
	}

	fprintf(fp21, "\n\n### FINAL connectivity\n");
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

	fclose(fp21);

	return 0;

error:
	return -1;
}
