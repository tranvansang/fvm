/*
 * INPUT
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "struct_ext.h"
#include "pcg_ext.h"
#include "input.h"

extern int
INPUT(void)
{
#define BUF_SIZE 1024
	char line[BUF_SIZE];
	char CNTFIL[81];
	double OMEGA;
	FILE *fp11;

/**************
 * CNTL. file *
 **************/
	if((fp11 = fopen("INPUT.DAT", "r")) == NULL) {
		fprintf(stderr, "Error: %s\n", strerror(errno));
		return -1;
	}

	fgets(line, BUF_SIZE, fp11);
	sscanf(line, "%d%d%d", &NX, &NY, &NZ);
	fgets(line, BUF_SIZE, fp11);
	sscanf(line, "%d", &METHOD);
	fgets(line, BUF_SIZE, fp11);
	sscanf(line, "%le%le%le", &DX, &DY, &DZ);
	fgets(line, BUF_SIZE, fp11);
	sscanf(line, "%le", &EPSICCG);
	fgets(line, BUF_SIZE, fp11);

	fclose(fp11);


	return 0;
}
