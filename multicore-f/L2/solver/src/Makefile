F90        = gfortran
F90OPTFLAGS= -O3

F90FLAGS =$(F90OPTFLAGS)

.SUFFIXES:
.SUFFIXES: .o .f .f90 .c 
#
.f90.o:; $(F90) -c $(F90FLAGS)  $(F90OPTFLAG) $<
.f.o:; $(F90) -c $(F90FLAGS)  $(F90OPTFLAG) $<
#
OBJS = \
solver_ICCG_mc.o cm.o rcm.o struct.o pcg.o \
boundary_cell.o cell_metrics.o \
input.o main.o poi_gen.o pointer_init.o outucd.o mc.o cmrcm.o

TARGET = ../run/L2-sol

all: $(TARGET)

$(TARGET): $(OBJS) 
	$(F90) $(F90FLAGS) -o $(TARGET) \
	$(OBJS) \
	$(F90FLAGS)

clean:
	rm -f *.o $(TARGET) *.mod *~ PI*
