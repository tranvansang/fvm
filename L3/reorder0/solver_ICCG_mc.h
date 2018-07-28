#ifndef __H_SOLVER_ICCG_MC
#define __H_SOLVER_ICCG_MC
/*
extern int
solve_ICCG_mc(int N, int NL, int NU, int *INL, int **IAL, int *INU, int **IAU,
		double *D, double *B, double *X, double **AL, double **AU,
		int NCOLOR, int PEsmpTOT, int *COLOR, int *COLORg,
		double EPS, int *ITR, int *IER);
*/

extern int
solve_ICCG_mc(int N, int NL, int NU, int *indexL, int *itemL, int *indexU, int *itemU,
		double *D, double *B, double *X, double *AL, double *AU,
		int NCOLOR, int PEsmpTOT, int *COLOR, int *COLORg,
		double EPS, int *ITR, int *IER);


#endif /* __H_SOLVER_ICCG_MC */
