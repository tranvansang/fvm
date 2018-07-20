!C    
!C***
!C*** RCM
!C***
!C
      subroutine RCM  (N, NL, NU, INL, IAL, INU, IAU, 
     &                 NCOLORtot, COLORindex, NEWtoOLD, OLDtoNEW)

      implicit REAL*8(A-H,O-Z)
      integer, dimension(N)   :: INL, INU, NEWtoOLD, OLDtoNEW
      integer, dimension(0:N) :: COLORindex
      integer, dimension(NL,N):: IAL
      integer, dimension(NU,N):: IAU

      integer, dimension(:,:), allocatable :: IW

      integer, dimension(:)  , allocatable :: INLw, INUw
      integer, dimension(:,:), allocatable :: IALw, IAUw

!C
!C +-------+
!C | INIT. |
!C +-------+
!C===
      allocate (IW(N,2))
 
      IW = 0

       INmin= N
      NODmin= 0

      do i= 1, N
        icon= 0
        do k= 1, INL(i)
          icon= icon + 1
        enddo
        do k= 1, INU(i)
          icon= icon + 1
        enddo

        if (icon.lt.INmin) then
          INmin = icon
          NODmin= i
        endif
      enddo
 200  continue


      if (NODmin.eq.0) NODmin= 1

      IW(NODmin,2)= 1

      NEWtoOLD(1     )= NODmin
      OLDtoNEW(NODmin)= 1

      icol= 1
!C===

!C
!C +----------+
!C | COLORING |
!C +----------+
!C===
      icouG= 1
      do icol= 2, N
        icou = 0
        icou0= 0
        do i= 1, N
          if (IW(i,2).eq.icol-1) then
            do k= 1, INL(i)
              in= IAL(k,i)
              if (IW(in,2).eq.0) then
                IW(in  ,2)= -icol
                icou      = icou + 1
                IW(icou,1)= in
              endif                                      
            enddo
            do k= 1, INU(i)
              in= IAU(k,i)
              if (IW(in,2).eq.0) then
                IW(in  ,2)= -icol
                icou      = icou + 1
                IW(icou,1)= in
              endif                                      
            enddo
          endif
        enddo

        if (icou.eq.0) then
          do i= 1, N
            if (IW(i,2).eq.0) then
              icou= icou + 1
              IW(i   ,2)= -icol
              IW(icou,1)= i
              goto 850
            endif
          enddo
        endif
 850    continue

        do ic= 1, icou
          inC= IW(ic,1)
          if (IW(inC,2).ne.0) then
            do k= 1, INL(inC)
              in= IAL(k,inC)
              if (IW(in,2).le.0) IW(in,2)= 0
            enddo
            do k= 1, INU(inC)
              in= IAU(k,inC)
              if (IW(in,2).le.0) IW(in,2)= 0
            enddo
          endif
        enddo

        do ic= 1, icou
          inC= IW(ic,1)
          if (IW(inC,2).ne.0) then
            icouG    = icouG + 1
            IW(inC,2)= icol
          endif
        enddo

        if (icouG.eq.N) exit
      enddo
!C===

!C
!C +----------------+
!C | FINAL COLORING |
!C +----------------+
!C===
 3000 continue
      NCOLORtot= icol
      icoug= 0
      do ic= NCOLORtot, 1, -1
        icou= 0
        do i= N, 1, -1
          if (IW(i,2).eq.ic) then
            icou = icou + 1
            icoug= icoug + 1
            NEWtoOLD(icoug)= i
            OLDtoNEW(i    )= icoug
          endif
        enddo
        COLORindex(NCOLORtot-ic+1)= icou
      enddo

      COLORindex(0)= 0
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
          IW(i,1) = IAL(j,NEWtoOLD(i))
        enddo
        do i= 1, N
          IAL(j,i) = IW(i,1)
        enddo
      enddo

      do j= 1, NU
        do i= 1, N
          IW(i,1) = IAU(j,NEWtoOLD(i))
        enddo
        do i= 1, N
          IAU(j,i) = IW(i,1)
        enddo
      enddo

      do i= 1, N
        IW(i,1) = INL(NEWtoOLD(i))
      enddo

      do i= 1, N
        INLw(i) = IW(i,1)
      enddo

      do i= 1, N
        IW(i,1) = INU(NEWtoOLD(i))
      enddo

      do i= 1, N
        INUw(i) = IW(i,1)
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



