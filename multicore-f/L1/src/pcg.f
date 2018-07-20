      module PCG

      integer, parameter :: N2= 256
      integer :: NUmax, NLmax, NCOLORtot, NCOLORk, NU, NL, METHOD
      integer :: NPU, NPL

      real(kind=8) :: EPSICCG

      real(kind=8), dimension(:), allocatable :: D, PHI, BFORCE
      real(kind=8), dimension(:), allocatable :: AL, AU

      integer, dimension(:), allocatable :: INL, INU, COLORindex
      integer, dimension(:), allocatable :: OLDtoNEW, NEWtoOLD

      integer, dimension(:,:), allocatable :: IAL, IAU

      integer, dimension(:), allocatable :: indexL, itemL
      integer, dimension(:), allocatable :: indexU, itemU

      end module PCG
