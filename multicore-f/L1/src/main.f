      program MAIN

      use STRUCT
      use PCG
      use solver_PCG
      use solver_ICCG
      use solver_ICCG2

      implicit REAL*8 (A-H,O-Z)

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

      if (METHOD.eq.1) then
        call solve_ICCG                                                 &
     &      ( ICELTOT, NPL, NPU, indexL, itemL, indexU, itemU, D,       &
     &        BFORCE,  PHI, AL, AU, EPSICCG, ITR, IER)
      endif

      if (METHOD.eq.2) then
        call solve_ICCG2                                                &
     &      ( ICELTOT, NPL, NPU, indexL, itemL, indexU, itemU, D,       &
     &        BFORCE,  PHI, AL, AU, EPSICCG, ITR, IER)
      endif

      if (METHOD.eq.3) then
        call solve_PCG                                                  &
     &      ( ICELTOT, NPL, NPU, indexL, itemL, indexU, itemU, D,       &
     &        BFORCE,  PHI, AL, AU, EPSICCG, ITR, IER)
      endif

      call OUTUCD

      stop
      end
