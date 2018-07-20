#ifndef __H_SOLVER_ICCG
#define __H_SOLVER_ICCG
/*
extern int
solve_ICCG(int N, int NL, int NU, int *INL, int **IAL, int *INU, int **IAU,
		double *D, double *B, double *X, double **AL, double **AU,
		double EPS, int *ITR, int *IER);
*/

extern int
solve_ICCG(int N, int NL, int NU, int *indexL, int *itemL, int *indexU, int *itemU,
		double *D, double *B, double *X, double *AL, double *AU,
		double EPS, int *ITR, int *IER);


#endif /* __H_SOLVER_ICCG */
