      program MAIN

      use STRUCT
      use PCG

      implicit REAL*8 (A-H,O-Z)

      call POINTER_INIT

      open (21, file='color.log', status='unknown')
      call POI_GEN

      call OUTUCD


        write (21,'(//,a,i8,/)') 'COLOR number', NCOLORtot
        do ic= 1, NCOLORtot
          do i= COLORindex(ic-1)+1, COLORindex(ic)
            write (21,'(3(a,i8))') '  #new', i, '   #old', NEWtoOLD(i), &
     &                             '  color', ic
          enddo
        enddo

        write (21,'(//,a)') '### FINAL connectivity'
        do i= 1, ICELTOT
          write (21,'(a,i5,a,i5,a,i5)') 'I=', i, '     INL(i)=', INL(i),&
     &                                           '     INU(i)=', INU(i)
          write (21,'(a, 6i5)') '  IAL:', (IAL(k,i), k= 1, INL(i))
          write (21,'(a, 6i5)') '  IAU:', (IAU(k,i), k= 1, INU(i))
        enddo

      close (21)

      stop
      end

