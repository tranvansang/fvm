!C
!C***
!C*** BOUNDARY_CELL
!C***
!C
      subroutine BOUNDARY_CELL
      use STRUCT

      implicit REAL*8 (A-H,O-Z)

!C
!C +------+
!C | Zmax |
!C +------+
!C=== 
      IFACTOT= NX * NY      
      ZmaxCELtot= IFACTOT
 
      allocate (ZmaxCEL(ZmaxCELtot))

      icou= 0      
      k   = NZ
      do j= 1, NY
      do i= 1, NX
        icel= (k-1)*IFACTOT + (j-1)*NX + i
        icou= icou + 1
        ZmaxCEL(icou)= icel
      enddo       
      enddo       
!C===
      return
      end
