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

         PHI= 0.d0
           D= 0.d0
      BFORCE= 0.d0

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
!C===

!C
!C +---------------+
!C | MULTICOLORING |
!C +---------------+
!C===
      allocate (OLDtoNEW(ICELTOT), NEWtoOLD(ICELTOT))
      allocate (COLORindex(0:ICELTOT))

 111    continue
        write (*,'(//a,i8,a)') 'You have', ICELTOT, ' elements.'
        write (*,'(  a     )') 'How many colors do you need ?'
        write (*,'(  a     )') '  #COLOR must be more than 2 and'
        write (*,'(  a,i8  )') '  #COLOR must not be more than', ICELTOT
        write (*,'(  a     )') '   CM if #COLOR .eq. 0'
        write (*,'(  a     )') '  RCM if #COLOR .eq.-1'
        write (*,'(  a     )') 'CMRCM if #COLOR .le.-2'
        write (*,'(  a     )') '=>'
        read  (*,*)             NCOLORtot
        if (NCOLORtot.eq.1.or.NCOLORtot.gt.ICELTOT) goto 111

      if (NCOLORtot.gt.0) then
        call MC  (ICELTOT, NL, NU, INL, IAL, INU, IAU,                  &
     &            NCOLORtot, COLORindex, NEWtoOLD, OLDtoNEW)
	write (*,'(//a)') '###  MultiCOloring' 
      endif

      if (NCOLORtot.eq.0) then
        call  CM (ICELTOT, NL, NU, INL, IAL, INU, IAU,                  &
     &            NCOLORtot, COLORindex, NEWtoOLD, OLDtoNEW)
	write (*,'(//a)') '###  CM' 
      endif

      if (NCOLORtot.eq.-1) then
        call RCM (ICELTOT, NL, NU, INL, IAL, INU, IAU,                  &
     &            NCOLORtot, COLORindex, NEWtoOLD, OLDtoNEW)
	write (*,'(//a)') '### RCM' 
      endif

      if (NCOLORtot.lt.-1) then
        call CMRCM (ICELTOT, NL, NU, INL, IAL, INU, IAU,                &
     &            NCOLORtot, COLORindex, NEWtoOLD, OLDtoNEW)
	write (*,'(//a)') '### CM-RCM' 
      endif

      write (*,'(//a,i8,// )') '### FINAL COLOR NUMBER', NCOLORtot

      icou= 0
      do i= 1, ICELTOT
        if (INL(i).eq.0) icou= icou + 1
      enddo
      write (*,*) icou

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

      do icol= 1, NCOLORtot
      do icel= COLORindex(icol-1)+1, COLORindex(icol)
        ic0 = NEWtoOLD(icel)

        icN1= NEIBcell(ic0,1)
        icN2= NEIBcell(ic0,2)
        icN3= NEIBcell(ic0,3)
        icN4= NEIBcell(ic0,4)
        icN5= NEIBcell(ic0,5)
        icN6= NEIBcell(ic0,6)

        VOL0= VOLCEL  (ic0)

        if (icN5.ne.0) then
          icN5= OLDtoNEW(icN5)
          coef= RDZ * ZAREA
          D(icel)= D(icel) - coef

          if (icN5.lt.icel) then
            do j= 1, INL(icel)
              if (IAL(j,icel).eq.icN5) then
                itemL(j+indexL(icel-1))= icN5
                   AL(j+indexL(icel-1))= coef
                exit
              endif
            enddo
           else
            do j= 1, INU(icel)
              if (IAU(j,icel).eq.icN5) then
                itemU(j+indexU(icel-1))= icN5
                   AU(j+indexU(icel-1))= coef
                exit
              endif
            enddo
          endif

        endif

        if (icN3.ne.0) then
          icN3= OLDtoNEW(icN3)
          coef= RDY * YAREA
          D(icel)= D(icel) - coef

          if (icN3.lt.icel) then
            do j= 1, INL(icel)
              if (IAL(j,icel).eq.icN3) then
                itemL(j+indexL(icel-1))= icN3
                   AL(j+indexL(icel-1))= coef
                exit
              endif
            enddo
           else
            do j= 1, INU(icel)
              if (IAU(j,icel).eq.icN3) then
                itemU(j+indexU(icel-1))= icN3
                   AU(j+indexU(icel-1))= coef
                exit
              endif
            enddo
          endif
        endif

        if (icN1.ne.0) then
          icN1= OLDtoNEW(icN1)
          coef= RDX * XAREA
          D(icel)= D(icel) - coef

          if (icN1.lt.icel) then
            do j= 1, INL(icel)
              if (IAL(j,icel).eq.icN1) then
                itemL(j+indexL(icel-1))= icN1
                   AL(j+indexL(icel-1))= coef
                exit
              endif
            enddo
           else
            do j= 1, INU(icel)
              if (IAU(j,icel).eq.icN1) then
                itemU(j+indexU(icel-1))= icN1
                   AU(j+indexU(icel-1))= coef
                exit
              endif
            enddo
          endif
        endif

        if (icN2.ne.0) then
          icN2= OLDtoNEW(icN2)
          coef= RDX * XAREA
          D(icel)= D(icel) - coef

          if (icN2.lt.icel) then
            do j= 1, INL(icel)
              if (IAL(j,icel).eq.icN2) then
                itemL(j+indexL(icel-1))= icN2
                   AL(j+indexL(icel-1))= coef
                exit
              endif
            enddo
           else
            do j= 1, INU(icel)
              if (IAU(j,icel).eq.icN2) then
                itemU(j+indexU(icel-1))= icN2
                   AU(j+indexU(icel-1))= coef
                exit
              endif
            enddo
          endif
        endif

        if (icN4.ne.0) then
          icN4= OLDtoNEW(icN4)
          coef= RDY * YAREA
          D(icel)= D(icel) - coef

          if (icN4.lt.icel) then
            do j= 1, INL(icel)
              if (IAL(j,icel).eq.icN4) then
                itemL(j+indexL(icel-1))= icN4
                   AL(j+indexL(icel-1))= coef
                exit
              endif
            enddo
           else
            do j= 1, INU(icel)
              if (IAU(j,icel).eq.icN4) then
                itemU(j+indexU(icel-1))= icN4
                   AU(j+indexU(icel-1))= coef
                exit
              endif
            enddo
          endif
        endif

        if (icN6.ne.0) then
          icN6= OLDtoNEW(icN6)
          coef= RDZ * ZAREA
          D(icel)= D(icel) - coef

          if (icN6.lt.icel) then
            do j= 1, INL(icel)
              if (IAL(j,icel).eq.icN6) then
                itemL(j+indexL(icel-1))= icN6
                   AL(j+indexL(icel-1))= coef
                exit
              endif
            enddo
           else
            do j= 1, INU(icel)
              if (IAU(j,icel).eq.icN6) then
                itemU(j+indexU(icel-1))= icN6
                   AU(j+indexU(icel-1))= coef
                exit
              endif
            enddo
          endif
        endif

        ii= XYZ(ic0,1)
        jj= XYZ(ic0,2)
        kk= XYZ(ic0,3)

        BFORCE(icel)= -dfloat(ii+jj+kk) * VOL0

      enddo
      enddo
!C===

!C
!C +--------------------------+
!C | DIRICHLET BOUNDARY CELLs |
!C +--------------------------+
!C   TOP SURFACE
!C===
      do ib= 1, ZmaxCELtot
        ic0= ZmaxCEL(ib)
        coef= 2.d0 * RDZ * ZAREA
        icel= OLDtoNEW(ic0)
        D(icel)= D(icel) - coef
      enddo
!C===

      return
      end
