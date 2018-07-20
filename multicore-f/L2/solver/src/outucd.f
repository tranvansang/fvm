!C
!C***
!C*** OUTUCD
!C***
!C
      subroutine OUTUCD
      use STRUCT
      use PCG

      implicit REAL*8 (A-H,O-Z)
      character*3  elem

      elem= 'hex'
      N0  = 0
      N1  = 1

      open (12, file= 'test.inp', status='unknown')

      write (12,'(5i8)')  N, ICELTOT, N0, N1, N0

      icou= 0
      do k= 1, NZP1
      do j= 1, NYP1
      do i= 1, NXP1
        icou= icou + 1
        XX= dfloat(i-1) * DX
        YY= dfloat(j-1) * DY
        ZZ= dfloat(k-1) * DZ
        write (12,'(i8,8x,3(1pe16.6))') icou, XX, YY, ZZ
      enddo
      enddo
      enddo

      icou= 0
      do k= 1, NZ
      do j= 1, NY
      do i= 1, NX
        icou= icou + 1
        iS = (k-1)*IBNODTOT + (j-1)*NXP1 + i
        in1= iS 
        in2= iS + 1
        in3= iS + 1 + NXP1
        in4= iS     + NXP1
        in5= in1 + IBNODTOT
        in6= in2 + IBNODTOT
        in7= in3 + IBNODTOT
        in8= in4 + IBNODTOT
        write (12,'(i8,i3,x,a3,2x,8i8)')  icou, N1, elem,               &
     &              in1, in2, in3, in4, in5, in6, in7, in8
      enddo
      enddo
      enddo

      write (12,'(2i8)') N1, N1
      write (12,'(a)')  'VAL,VAL'
      do i= 1, ICELTOT
        write (12,'(i8,1pe16.6)') i, PHI(i)
      enddo

      close (12)

      return
      end
