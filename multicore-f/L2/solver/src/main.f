      program MAIN

      use STRUCT
      use PCG
      use solver_ICCG_mc

      implicit REAL*8 (A-H,O-Z)
      real(kind=8), dimension(:), allocatable :: WK
      real(kind=4) ::  time, t1, t2, tarray(2)

!C     
!C-- INIT.
      call INPUT
      call POINTER_INIT
      call BOUNDARY_CELL
      call CELL_METRICS
      call POI_GEN

!C 
!C-- MAIN SOLVER
      PHI=  0.d0

      ISET= 0

      allocate (WK(ICELTOT))

        call solve_ICCG_mc                                              &
     &      ( ICELTOT, NPL, NPU, indexL, itemL, indexU, itemU, D,       &
     &        BFORCE,  PHI, AL, AU, NCOLORtot, COLORindex,              &
     &        EPSICCG, ITR, IER)

      xN= dfloat(ICELTOT)
      xL= dfloat(NPL)
      xU= dfloat(NPL)

      FLOPS= xN + 3.d0*xL + 2.d0*(xL+xU+xN) + 2.d0*xN
      FLOPS= FLOPS+dfloat(ITR)*(4.d0*xL+16.d0*xN+4.d0*xU+2.d0) -2.d0*xN

      write (*,'(//, a, i10)') 'N= ', ICELTOT

      do ic0= 1, ICELTOT
        icel= NEWtoOLD(ic0)
        WK(icel)= PHI(ic0)
      enddo

      do icel= 1, ICELTOT
        PHI(icel)= WK(icel)
      enddo
      call OUTUCD

      stop
      end
