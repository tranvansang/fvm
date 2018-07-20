!C    
!C***
!C*** MC
!C***
!C
!C    Multicolor Ordering Method
!C    

      subroutine MC (N, NL, NU, INL, IAL, INU, IAU,                     &
     &               NCOLORtot, COLORindex, NEWtoOLD, OLDtoNEW)

      implicit REAL*8(A-H,O-Z)

      integer, dimension(N)   :: INL, INU, NEWtoOLD, OLDtoNEW
      integer, dimension(0:N) :: COLORindex
      integer, dimension(NL,N):: IAL
      integer, dimension(NU,N):: IAU

      integer, dimension(:)  , allocatable :: IW, INLw, INUw
      integer, dimension(:,:), allocatable :: IALw, IAUw

!C
!C +-------+
!C | INIT. |
!C +-------+
!C===
      allocate (IW(N))
      IW= 0

      NCOLORk  = NCOLORtot

      do i= 1, N
        NEWtoOLD  (i)= i
        OLDtoNEW  (i)= i
      enddo

       INmin= N
      NODmin= 0
      do i= 1, N
        icon= INL(i) + INU(i)
        if (icon.lt.INmin) then
           INmin= icon
          NODmin= i
        endif
      enddo

      OLDtoNEW(NODmin)= 1
      NEWtoOLD(     1)= NODmin

      IW        = 0
      IW(NODmin)= 1

      ITEMcou= N/NCOLORk
!C===

!C
!C +---------------+
!C | MULTIcoloring |
!C +---------------+
!C===
      icou = 1
      icouK= 1

      do icol= 1, N
        NCOLORk= icol
        do i= 1, N
          if (IW(i).le.0) IW(i)= 0
        enddo
        do i= 1, N
!C
!C-- already COLORED
          if (IW(i).eq.icol) then
            do k= 1, INL(i)
              ik= IAL(k,i)
              if (IW(ik).le.0) IW(ik)= -1
            enddo
            do k= 1, INU(i)
              ik= IAU(k,i)
              if (IW(ik).le.0) IW(ik)= -1
            enddo
          endif
!C
!C-- not COLORED
          if (IW(i).eq.0) then
            icou = icou  + 1
            icouK= icouK + 1
            IW(i)= icol
            do k= 1, INL(i)
              ik= IAL(k,i)
              if (IW(ik).le.0) IW(ik)= -1
            enddo
            do k= 1, INU(i)
              ik= IAU(k,i)
              if (IW(ik).le.0) IW(ik)= -1
            enddo
          endif
          if (icou .eq.N)       goto 200
          if (icouK.eq.ITEMcou) goto 100
        enddo      
 100    continue
        icouK= 0
      enddo

 200  continue
!C===

!C
!C +----------------+
!C | FINAL COLORING |
!C +----------------+
!C===
      NCOLORtot= NCOLORk
      COLORindex= 0
      icoug= 0
      do ic= 1, NCOLORtot
        icou= 0
        do i= 1, N
          if (IW(i).eq.ic) then
            icou = icou + 1
            icoug= icoug + 1
            NEWtoOLD(icoug)= i
            OLDtoNEW(i    )= icoug
          endif
        enddo
        COLORindex(ic)= icou
      enddo

      do ic= 1, NCOLORtot
        COLORindex(ic)= COLORindex(ic-1) + COLORindex(ic)
      enddo
!C===

!C
!C +-----------------+
!C | MATRIX transfer |
!C +-----------------+
!C===
      allocate (INLw(N), INUw(N), IALw(NL,N), IAUw(NU,N))
      do j= 1, NL
        do i= 1, N
          IW(i) = IAL(j,NEWtoOLD(i))
        enddo
        do i= 1, N
          IAL(j,i) = IW(i)
        enddo
      enddo

      do j= 1, NU
        do i= 1, N
          IW(i) = IAU(j,NEWtoOLD(i))
        enddo
        do i= 1, N
          IAU(j,i) = IW(i)
        enddo
      enddo

      do i= 1, N
        IW(i) = INL(NEWtoOLD(i))
      enddo

      do i= 1, N
        INLw(i) = IW(i)
      enddo

      do i= 1, N
        IW(i) = INU(NEWtoOLD(i))
      enddo

      do i= 1, N
        INUw(i) = IW(i)
      enddo

      do j= 1, NL
        do i= 1, N
          if (IAL(j,i).eq.0) then
            IALw(j,i) = 0
           else
            IALw(j,i) = OLDtoNEW(IAL(j,i))
          endif
        enddo
      enddo

      do j= 1, NU
        do i= 1, N
          if ( IAU(j,i).eq.0) then
            IAUw(j,i) = 0
           else
            IAUw(j,i) = OLDtoNEW(IAU(j,i))
          endif
        enddo
      enddo

      INL= 0
      INU= 0
      IAL= 0
      IAU= 0

      do i= 1, N
        jL= 0
        jU= 0
        do j= 1, INLw(i)
          if (IALw(j,i).gt.i) then
            jU= jU + 1
            IAU(jU,i)= IALw(j,i)
           else
            jL= jL + 1
            IAL(jL,i)= IALw(j,i)
          endif
         enddo

        do j= 1, INUw(i)
          if (IAUw(j,i).gt.i) then
            jU= jU + 1
            IAU(jU,i)= IAUw(j,i)
           else
            jL= jL + 1
            IAL(jL,i)= IAUw(j,i)
          endif
         enddo

         INL(i)= jL
         INU(i)= jU
      enddo
!C===
      deallocate (IW, INLw, INUw, IALw, IAUw)

      return
      end



