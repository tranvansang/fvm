#ifndef __H_SOLVER_PCG
#define __H_SOLVER_PCG
/*
extern int
solve_PCG      (int N, int NL, int NU, int *INL, int **IAL, int *INU, int **IAU,
		double *D, double *B, double *X, double **AL, double **AU,
		double EPS, int *ITR, int *IER, int N2);
*/

extern int
solve_PCG (int N, int NL, int NU, int *indexL, int *itemL, int *indexU, int *itemU,
		double *D, double *B, double *X, double *AL, double *AU,
	        double EPS, int *ITR, int *IER, int N2);


#endif /* __H_SOLVER_PCG */
