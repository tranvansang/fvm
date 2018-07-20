#ifndef __H_PCG_EXT
#define __H_PCG_EXT

	extern int N2;
	extern int NUmax, NLmax, NCOLORtot, NCOLORk, NU, NL;
	extern int METHOD, ORDER_METHOD;

	extern double EPSICCG;

        extern double *D, *PHI, *BFORCE;
        //extern double **AL, **AU;
        extern double *AL, *AU;

        extern int *INL, *INU, *COLORindex;
        extern int *indexL, *indexU;
        extern int *indexLorg, *indexUorg;
        extern int *SMPindex, *SMPindexG;
        extern int *OLDtoNEW, *NEWtoOLD;

        extern int **IAL, **IAU;
        extern int *itemL, *itemU;
        extern int *itemLorg, *itemUorg;
     
        extern int NPL, NPU;

	

#endif /* __H_PCG_EXT */
