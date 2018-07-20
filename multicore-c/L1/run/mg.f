      integer, dimension(0:6) :: BC_INDEX
      integer, dimension(:,:), allocatable :: NEIBcell, XYZ

      character(len= 9 )   ::  fname
!C
!C +-------+
!C | INIT. |
!C +-------+
!C===
      read (*,*) NX, NY, NZ

!      NY= NX
!      NZ= NX

      ICELTOT= NX  * NY  * NZ

      NXP1= NX + 1
      NYP1= NY + 1
      NZP1= NZ + 1

      allocate (NEIBcell(ICELTOT,6), XYZ(ICELTOT,3))      
!C===

!C
!C +-------------------+
!C | INDIVIDUAL REGION |
!C +-------------------+
!C===
      NEIBcell= 0
!C
!C-- NEIGHBORING CELLs
      do k= 1, NZ
        do j= 1, NY
          do i= 1, NX
            icel= (k-1)*NX*NY + (j-1)*NX + i
            NEIBcell(icel,1)= icel - 1
            NEIBcell(icel,2)= icel + 1
            NEIBcell(icel,3)= icel - NX
            NEIBcell(icel,4)= icel + NX
            NEIBcell(icel,5)= icel - NX*NY
            NEIBcell(icel,6)= icel + NX*NY
            if (i.eq. 1) NEIBcell(icel,1)= 0
            if (i.eq.NX) NEIBcell(icel,2)= 0
            if (j.eq. 1) NEIBcell(icel,3)= 0
            if (j.eq.NY) NEIBcell(icel,4)= 0
            if (k.eq. 1) NEIBcell(icel,5)= 0
            if (k.eq.NZ) NEIBcell(icel,6)= 0

            XYZ(icel,1)= i
            XYZ(icel,2)= j
            XYZ(icel,3)= k

          enddo
        enddo
      enddo
             
!C
!C-- POST processing
      ipe= 1
      open (21, file= 'mesh.dat', status='unknown', form='formatted')

      write (21,'(10i10)') NX, NY, NZ
      write (21,'(10i10)') ICELTOT

      do i= 1, ICELTOT
        write (21,'(10i10)') i, (NEIBcell(i,k), k= 1, 6),               &
     &                          (XYZ     (i,j), j= 1, 3)
      enddo

      close (21)
!C===
      stop
      end
