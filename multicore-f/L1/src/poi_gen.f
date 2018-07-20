!C
!C***
!C*** POI_GEN
!C***
!C
!C    generate COEF. MATRIX for POISSON equations
!C    
      subroutine POI_GEN

      use STRUCT
      use PCG

      implicit REAL*8 (A-H,O-Z)

!C
!C-- INIT.
      nn = ICELTOT

      NL= 6
      NU= 6

      allocate (BFORCE(nn), D(nn), PHI(nn))
      allocate (INL(nn), INU(nn), IAL(NL,nn), IAU(NU,nn))

      PHI   = 0.d0
      BFORCE= 0.d0
        D   = 0.d0

      INL= 0
      INU= 0
      IAL= 0
      IAU= 0

!C
!C-- INTERIOR & NEUMANN boundary's
      do icel= 1, ICELTOT
        icN1= NEIBcell(icel,1)
        icN2= NEIBcell(icel,2)
        icN3= NEIBcell(icel,3)
        icN4= NEIBcell(icel,4)
        icN5= NEIBcell(icel,5)
        icN6= NEIBcell(icel,6)

        icouG= 0

        if (icN5.ne.0) then
          icou= INL(icel) + 1
          IAL(icou,icel)= icN5
          INL(     icel)= icou
        endif

        if (icN3.ne.0) then
          icou= INL(icel) + 1
          IAL(icou,icel)= icN3
          INL(     icel)= icou
        endif

        if (icN1.ne.0) then
          icou= INL(icel) + 1
          IAL(icou,icel)= icN1
          INL(     icel)= icou
        endif

        if (icN2.ne.0) then
          icou= INU(icel) + 1
          IAU(icou,icel)= icN2
          INU(     icel)= icou
        endif

        if (icN4.ne.0) then
          icou= INU(icel) + 1
          IAU(icou,icel)= icN4
          INU(     icel)= icou
        endif

        if (icN6.ne.0) then
          icou= INU(icel) + 1
          IAU(icou,icel)= icN6
          INU(     icel)= icou
        endif
      enddo

!C
!C-- 1D array
      allocate (indexL(0:nn), indexU(0:nn))
      indexL= 0
      indexU= 0
     
      do icel= 1, ICELTOT
        indexL(icel)= INL(icel)
        indexU(icel)= INU(icel)
      enddo

      do icel= 1, ICELTOT
        indexL(icel)= indexL(icel) + indexL(icel-1)
        indexU(icel)= indexU(icel) + indexU(icel-1)
      enddo

      NPL= indexL(ICELTOT)
      NPU= indexU(ICELTOT)

      allocate (itemL(NPL), AL(NPL))
      allocate (itemU(NPU), AU(NPU))

      itemL= 0
      itemU= 0
         AL= 0.d0
         AU= 0.d0
!C===

!C
!C +-----------------------------------+
!C | INTERIOR & NEUMANN BOUNDARY CELLs |
!C +-----------------------------------+
!C===
      icouG= 0
      do icel= 1, ICELTOT
        icN1= NEIBcell(icel,1)
        icN2= NEIBcell(icel,2)
        icN3= NEIBcell(icel,3)
        icN4= NEIBcell(icel,4)
        icN5= NEIBcell(icel,5)
        icN6= NEIBcell(icel,6)

        VOL0= VOLCEL(icel)

        icou= 0
        if (icN5.ne.0) then
          coef   =RDZ * ZAREA
          D(icel)= D(icel) - coef
                   icou= icou + 1
                   k   = icou + indexL(icel-1)
          itemL(k)= icN5
             AL(k)= coef
        endif

        if (icN3.ne.0) then
          coef   = RDY * YAREA
          D(icel)= D(icel) - coef
                   icou= icou + 1
                   k   = icou + indexL(icel-1)
          itemL(k)= icN3
             AL(k)= coef
        endif

        if (icN1.ne.0) then
          coef   = RDX * XAREA
          D(icel)= D(icel) - coef
                   icou= icou + 1
                   k   = icou + indexL(icel-1)
          itemL(k)= icN1
             AL(k)= coef
        endif

        icou= 0
        if (icN2.ne.0) then
          coef   = RDX * XAREA
          D(icel)= D(icel) - coef
                   icou= icou + 1
                   k   = icou + indexU(icel-1)
          itemU(k)= icN2
             AU(k)= coef
        endif

        if (icN4.ne.0) then
          coef   = RDY * YAREA
          D(icel)= D(icel) - coef
                   icou= icou + 1
                   k   = icou + indexU(icel-1)
          itemU(k)= icN4
             AU(k)= coef
        endif

        if (icN6.ne.0) then
          coef   = RDZ * ZAREA
          D(icel)= D(icel) - coef
                   icou= icou + 1
                   k   = icou + indexU(icel-1)
          itemU(k)= icN6
             AU(k)= coef
        endif

        ii= XYZ(icel,1)
        jj= XYZ(icel,2)
        kk= XYZ(icel,3)

        BFORCE(icel)= -dfloat(ii+jj+kk) * VOL0

      enddo
!C===

!C
!C +--------------------------+
!C | DIRICHLET BOUNDARY CELLs |
!C +--------------------------+
!C   TOP SURFACE
!C===
      do ib= 1, ZmaxCELtot
        icel= ZmaxCEL(ib)
        coef= 2.d0 * RDZ * ZAREA
        D(icel)= D(icel) - coef
      enddo
!C===

      return
      end
