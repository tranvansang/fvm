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

      NU= 6
      NL= 6

      allocate (INL(nn), INU(nn), IAL(NL,nn), IAU(NU,nn))

      INL= 0
      INU= 0
      IAL= 0
      IAU= 0

!C
!C +--------------+
!C | CONNECTIVITY |
!C +--------------+
!C===
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
!C +----------------------+
!C | INITIAL connectivity |
!C +----------------------+
!C===
      write (21,'(a)') '### INITIAL connectivity'
      do i= 1, ICELTOT
        write (21,'(a,i5,a,i5,a,i5)') 'I=', i, '     INL(i)=', INL(i),  &
     &                                         '     INU(i)=', INU(i)
        write (21,'(a, 6i5)') '  IAL:', (IAL(k,i), k= 1, INL(i))
        write (21,'(a, 6i5)') '  IAU:', (IAU(k,i), k= 1, INU(i))
      enddo
!C===

!C
!C +---------------+
!C | MULTICOLORING |
!C +---------------+
!C===
 111  continue
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

      allocate (OLDtoNEW(ICELTOT), NEWtoOLD(ICELTOT))
      allocate (COLORindex(0:ICELTOT))

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
!C===

      write (*,'(/a, i8)') '# TOTAL COLOR number', NCOLORtot

      return
      end
