      implicit REAL*8 (A-H,O-Z)
 
      parameter (NDIM= 12, N= 12)

      real*8 A(NDIM,N), B(N)
      real*8 Y(N)

!C
!C +-------+
!C | INIT. |
!C +-------+
!C===
      do j= 1, N
      do i= 1, N
        A(i,j)= 0.d0
      enddo
      enddo

      A( 1, 2)= -1.d0
      A( 1, 4)= -1.d0
      A( 2, 1)= -1.d0
      A( 2, 3)= -1.d0
      A( 2, 5)= -1.d0
      A( 3, 2)= -1.d0
      A( 3, 6)= -1.d0
      A( 4, 1)= -1.d0
      A( 4, 5)= -1.d0
      A( 4, 7)= -1.d0
      A( 5, 2)= -1.d0
      A( 5, 4)= -1.d0
      A( 5, 6)= -1.d0
      A( 5, 8)= -1.d0
      A( 6, 3)= -1.d0
      A( 6, 5)= -1.d0
      A( 6, 9)= -1.d0
      A( 7, 4)= -1.d0
      A( 7, 8)= -1.d0
      A( 7,10)= -1.d0
      A( 8, 5)= -1.d0
      A( 8, 7)= -1.d0
      A( 8, 9)= -1.d0
      A( 8,11)= -1.d0
      A( 9, 6)= -1.d0
      A( 9, 8)= -1.d0
      A( 9,12)= -1.d0
      A(10, 7)= -1.d0
      A(10,11)= -1.d0
      A(11, 8)= -1.d0
      A(11,10)= -1.d0
      A(11,12)= -1.d0
      A(12, 9)= -1.d0
      A(12,11)= -1.d0

      do i= 1, N
        A(i,i)= 6.d0
      enddo

      do i= 1, N
        VAL= 0.d0
        do j= 1, N
          VAL= VAL + A(i,j)*dfloat(j)
        enddo
        B(i)= VAL
      enddo
!C===

!C
!C +--------+
!C | MATRIX |
!C +--------+
!C===
      write (*,'(/a)') '### ORIGINAL MATRIX ###'
      do i= 1, N
        write (*,'(13f6.2)') (A(i,j), j= 1, N), B(i)
      enddo 
!C===

!C
!C +------------------+
!C | LU decomposition |
!C +------------------+
!C===
      do k= 1, N
        do j= k, N
          s= 0.d0
          if (k.ne.1) then
            do ll= 1, k-1
              s= s + A(k,ll)*A(ll,j)
            enddo
          endif
          A(k,j)= A(k,j) - s
        enddo

        if (k.ne.N) then
          do i= k+1, N
            s= 0.d0
            if (k.ne.1) then
              do ll= 1, k-1
                s= s + A(i,ll)*A(ll,k)
              enddo
            endif
            A(i,k)= (A(i,k)-s)/A(k,k)
          enddo
        endif
      enddo
        
      write (*,'(//a)') '### LU decomposed MATRIX ###'
      do i= 1, N
        write (*,'(13f6.2)') (A(i,j), j= 1, N), B(i)
      enddo 
!C===

!C
!C +------------------------------+
!C | Foward/Backward Substitution |
!C +------------------------------+
!C===
      do i= 1, N
        Y(i)= B(i)
      enddo

      do i= 2, N
        s= 0.d0
        do ll= 1, i-1
          s= s + A(i,ll)*Y(ll)
        enddo
        Y(i)= Y(i) - s
      enddo

      B(N)= Y(N)/A(N,N)
      do i= N-1, 1, -1
        s= 0.d0
        do ll= i+1, N
          s= s + A(i,ll)*B(ll)
        enddo
        B(i)= (Y(i)-s)/A(i,i)
      enddo

      write (*,'(//a)') '### RESULTS ###'
      do i= 1, N
        write (*,'(i8, 1pe16.6)') i, B(i)
      enddo
!C===
      stop
      end
