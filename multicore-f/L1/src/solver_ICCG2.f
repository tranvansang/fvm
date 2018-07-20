!C
!C*** 
!C*** module solver_ICCG2
!C***
!
      module solver_ICCG2
      contains
!C
!C*** solve_ICCG2
!C
      subroutine solve_ICCG2                                            &
     &         ( N, NPL, NPU, indexL, itemL, indexU, itemU, D, B, X,    &
     &           AL, AU, EPS, ITR, IER)

      implicit REAL*8 (A-H,O-Z)

      real(kind=8), dimension(N)   :: D
      real(kind=8), dimension(N)   :: B
      real(kind=8), dimension(N)   :: X
      real(kind=8), dimension(NPL) :: AL
      real(kind=8), dimension(NPU) :: AU

      integer, dimension(0:N)   :: indexL, indexU
      integer, dimension(NPL):: itemL
      integer, dimension(NPU):: itemU

      real(kind=8), dimension(:,:), allocatable :: W

      integer, parameter ::  R= 1
      integer, parameter ::  Z= 2
      integer, parameter ::  Q= 2
      integer, parameter ::  P= 3
      integer, parameter :: DD= 4

      real(kind=8), dimension(:), allocatable :: ALlu0, AUlu0
      real(kind=8), dimension(:), allocatable :: Dlu0

!C
!C +------+
!C | INIT |
!C +------+
!C===
      allocate (W(N,4))

      do i= 1, N
        X(i)  = 0.d0
        W(i,1)= 0.0D0
        W(i,2)= 0.0D0
        W(i,3)= 0.0D0
        W(i,4)= 0.0D0
      enddo

      call FORM_ILU0
!C===

!C
!C +-----------------------+
!C | {r0}= {b} - [A]{xini} |
!C +-----------------------+
!C===
      do i= 1, N
        VAL= D(i)*X(i)
        do k= indexL(i-1)+1, indexL(i)
          VAL= VAL + AL(k)*X(itemL(k))
        enddo 
        do k= indexU(i-1)+1, indexU(i)
          VAL= VAL + AU(k)*X(itemU(k))
        enddo 
        W(i,R)= B(i) - VAL
      enddo

      BNRM2= 0.0D0
      do i= 1, N
        BNRM2 = BNRM2 + B(i)  **2
      enddo
!C===

!C
!C***************************************************************  ITERATION
      ITR= N

      do L= 1, ITR
!C
!C +----------------+
!C | {z}= [Minv]{r} |
!C +----------------+
!C===      
      do i= 1, N
        W(i,Z)= W(i,R)
      enddo

      do i= 1, N
        WVAL= W(i,Z)
        do k= indexL(i-1)+1, indexL(i)
          WVAL= WVAL -  ALlu0(k) * W(itemL(k),Z)
        enddo
        W(i,Z)= WVAL * Dlu0(i)
      enddo

      do i= N, 1, -1
        SW  = 0.0d0
        do k= indexU(i-1)+1, indexU(i)
          SW= SW + AUlu0(k) * W(itemU(k),Z)
        enddo
        W(i,Z)= W(i,Z) - Dlu0(i)*SW
      enddo
!C===

!C
!C +-------------+
!C | RHO= {r}{z} |
!C +-------------+
!C===
      RHO= 0.d0
      do i= 1, N
        RHO= RHO + W(i,R)*W(i,Z)   
      enddo          
!C===

!C
!C +-----------------------------+
!C | {p} = {z} if      ITER=1    |
!C | BETA= RHO / RHO1  otherwise |
!C +-----------------------------+
!C===
      if ( L.eq.1 ) then

        do i= 1, N
          W(i,P)= W(i,Z)
        enddo
       else
         BETA= RHO / RHO1
         do i= 1, N
           W(i,P)= W(i,Z) + BETA*W(i,P)
         enddo
      endif
!C===

!C
!C +-------------+
!C | {q}= [A]{p} |
!C +-------------+
!C===        
      do i= 1, N
        VAL= D(i)*W(i,P)
        do k= indexL(i-1)+1, indexL(i)
          VAL= VAL + AL(k)*W(itemL(k),P)
        enddo 
        do k= indexU(i-1)+1, indexU(i)
          VAL= VAL + AU(k)*W(itemU(k),P)
        enddo 
        W(i,Q)= VAL
      enddo
!C===

!C
!C +---------------------+
!C | ALPHA= RHO / {p}{q} |
!C +---------------------+
!C===
      C1= 0.d0
      do i= 1, N
        C1= C1 + W(i,P)*W(i,Q)
      enddo

      ALPHA= RHO / C1
!C===


!C
!C +----------------------+
!C | {x}= {x} + ALPHA*{p} |
!C | {r}= {r} - ALPHA*{q} |
!C +----------------------+
!C===
      do i= 1, N
        X(i)  = X(i)   + ALPHA * W(i,P)
        W(i,R)= W(i,R) - ALPHA * W(i,Q)
      enddo

      DNRM2= 0.d0
      do i= 1, N
        DNRM2= DNRM2 + W(i,R)**2
      enddo
!C===

      ERR = dsqrt(DNRM2/BNRM2)
      if (mod(L,100).eq.1) then
        write (*,'(i5,2(1pe16.6))') L, ERR
      endif

        if (ERR .lt. EPS) then
          IER = 0
          goto 900
         else
          RHO1 = RHO
        endif

      enddo
      IER = 1

  900 continue
      write (*,'(i5,2(1pe16.6))') L, ERR

      ITR= L
!      EPS= ERR

      deallocate (W, Dlu0, ALlu0, AUlu0)

      contains

!C
!C***
!C*** FORM_ILU0
!C***
!C
!C    form ILU(0) matrix
!C
      subroutine FORM_ILU0
      implicit REAL*8 (A-H,O-Z)
      integer, dimension(:), allocatable :: IW1 , IW2
      integer, dimension(:), allocatable :: IWsL, IWsU
      real (kind=8):: RHS_Aij, DkINV, Aik, Akj

!C
!C +-------+
!C | INIT. |
!C +-------+
!C===
      allocate (ALlu0(NPL), AUlu0(NPU))
      allocate (Dlu0(N))

      do i= 1, N
        Dlu0(i)= D(i)
        do k= indexL(i-1)+1, indexL(i)
          ALlu0(k)= AL(k)
        enddo

        do k= indexU(i-1)+1, indexU(i)
          AUlu0(k)= AU(k)
        enddo
      enddo
!C===

!C
!C +----------------------+
!C | ILU(0) factorization |
!C +----------------------+
!C===
      allocate (IW1(N) , IW2(N))
      IW1= 0
      IW2= 0


      do i= 1, N
        do k0= indexL(i-1)+1, indexL(i)
          IW1(itemL(k0))= k0
        enddo

        do k0= indexU(i-1)+1, indexU(i)
          IW2(itemU(k0))= k0
        enddo

        do icon= indexL(i-1)+1, indexL(i)
            k= itemL(icon)
          D11= Dlu0(k)

          DkINV= 1.d0/D11
            Aik= ALlu0(icon)

          do kcon= indexU(k-1)+1, indexU(k)
            j= itemU(kcon)

            if (j.eq.i) then
                  Akj= AUlu0(kcon)
              RHS_Aij= Aik * DkINV * Akj
              Dlu0(i)= Dlu0(i) - RHS_Aij
            endif   

            if (j.lt.i .and. IW1(j).ne.0) then
                  Akj= AUlu0(kcon)
              RHS_Aij= Aik * DkINV * Akj

                      ij0 = IW1(j)
                ALlu0(ij0)= ALlu0(ij0) - RHS_Aij
            endif   

            if (j.gt.i .and. IW2(j).ne.0) then
                  Akj= AUlu0(kcon)
              RHS_Aij= Aik * DkINV * Akj

                    ij0 = IW2(j)
              AUlu0(ij0)= AUlu0(ij0) - RHS_Aij
            endif   

          enddo
	enddo

        do k0= indexL(i-1)+1, indexL(i)
          IW1(itemL(k0))= 0
        enddo

        do k0= indexU(i-1)+1, indexU(i)
          IW2(itemU(k0))= 0
        enddo

      enddo

      do i= 1, N
        Dlu0(i)= 1.d0 / Dlu0(i)
      enddo

      deallocate (IW1, IW2)
!C===
      end subroutine FORM_ILU0

      end subroutine  solve_ICCG2
      end module     solver_ICCG2
