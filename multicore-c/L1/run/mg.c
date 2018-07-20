#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int
main(int argc, char **argv)
{
	int NX, NY, NZ, NXP1, NYP1, NZP1, ICELTOT;
	int BC_INDEX[7];
	int **NEIBcell, **XYZ;
	char fname[10];
	int icel, ipe, i, j, k;
	FILE *fp21;

	fscanf(stdin, "%d%d%d", &NX, &NY, &NZ);

/*
 * INIT.
 */
	ICELTOT = NX * NY * NZ;

	NXP1 = NX + 1;
	NYP1 = NY + 1;
	NZP1 = NZ + 1;

	NEIBcell = (int **)calloc(ICELTOT, sizeof(int *));
	if(NEIBcell == NULL) {
		fprintf(stderr, "Error: %s\n", strerror(errno));
		return -1;
	}
	for(i=0; i<ICELTOT; i++) {
		NEIBcell[i] = (int *)calloc(6, sizeof(int));
		if(NEIBcell[i] == NULL) {
			fprintf(stderr, "Error: %s\n", strerror(errno));
			return -1;
		}
	}

	XYZ = (int **)calloc(ICELTOT, sizeof(int *));
	if(XYZ == NULL) {
		fprintf(stderr, "Error: %s\n", strerror(errno));
		return -1;
	}
	for(i=0; i<ICELTOT; i++) {
		XYZ[i] = (int *)calloc(3, sizeof(int));
		if(XYZ[i] == NULL) {
			fprintf(stderr, "Error: %s\n", strerror(errno));
			return -1;
		}
	}

/*
 * INDIVIDUAL REGION
 */
	for(k=0; k<NZ; k++) {
		for(j=0; j<NY; j++) {
			for(i=0; i<NX; i++) {
				icel = k * NX * NY + j * NX + i;
				NEIBcell[icel][0] = icel - 1       + 1;
				NEIBcell[icel][1] = icel + 1       + 1;
				NEIBcell[icel][2] = icel - NX      + 1;
				NEIBcell[icel][3] = icel + NX      + 1;
				NEIBcell[icel][4] = icel - NX * NY + 1;
				NEIBcell[icel][5] = icel + NX * NY + 1;
				if(i == 0)    NEIBcell[icel][0] = 0;
				if(i == NX-1) NEIBcell[icel][1] = 0;
				if(j == 0)    NEIBcell[icel][2] = 0;
				if(j == NY-1) NEIBcell[icel][3] = 0;
				if(k == 0)    NEIBcell[icel][4] = 0;
				if(k == NZ-1) NEIBcell[icel][5] = 0;

				XYZ[icel][0] = i + 1;
				XYZ[icel][1] = j + 1;
				XYZ[icel][2] = k + 1;
			}
		}
	}

/*
 * POST processing
 */
	ipe = 1;
	sprintf(fname, "mesh.dat", ipe-1);

	if((fp21 = fopen(fname, "w")) == NULL) {
		fprintf(stderr, "Error: %s\n", strerror(errno));
		return -1;
	}

	fprintf(fp21, "%10d%10d%10d\n", NX, NY, NZ);
	fprintf(fp21, "%10d\n", ICELTOT);

	for(i=0; i<ICELTOT; i++) {
		fprintf(fp21, "%10d", i+1);
		for(k=0; k<6; k++) {
			fprintf(fp21, "%10d", NEIBcell[i][k]);
		}
		for(j=0; j<3; j++) {
			fprintf(fp21, "%10d", XYZ[i][j]);
		}
		fprintf(fp21, "\n");
	}

	fclose(fp21);

	return 0;
}
