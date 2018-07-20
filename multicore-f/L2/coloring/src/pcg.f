      module PCG

      integer, parameter :: N2= 256
      integer :: NUmax, NLmax, NCOLORtot, NCOLORk, NU, NL

      real(kind=8) :: EPSICCG

      real(kind=8), dimension(:  ), allocatable :: D, DELPHI, BFORCE
      real(kind=8), dimension(:,:), allocatable :: AL, AU

      integer, dimension(:), allocatable :: INL, INU, COLORindex
      integer, dimension(:), allocatable :: OLDtoNEW, NEWtoOLD

      integer, dimension(:,:), allocatable :: IAL, IAU

      end module PCG
