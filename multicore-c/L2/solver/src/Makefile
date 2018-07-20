CC        = cc
#OPTFLAGS  = -Kfast,openmp -Nsrc
OPTFLAGS  = -O3 -lm
TARGET    = ../run/L2-sol

.SUFFIXES:
.SUFFIXES: .o .c 

.c.o:
	$(CC) -c $(CFLAGS) $(OPTFLAGS) $< -o $@

OBJS = \
	input.o \
	pointer_init.o \
	boundary_cell.o \
	cell_metrics.o \
	mc.o \
	cm.o \
	rcm.o \
	cmrcm.o \
	poi_gen.o \
	solver_ICCG_mc.o \
	outucd.o \
	allocate.o \
	main.o

HEADERS = \
	struct.h \
	struct_ext.h \
	pcg.h \
	pcg_ext.h \
	input.h \
	pointer_init.h \
	boundary_cell.h \
	cell_metrics.h \
	mc.h \
	cm.h \
	rcm.h \
	cmrcm.h \
	poi_gen.h \
	solver_ICCG_mc.h \
	allocate.h \
	outucd.h

all: $(TARGET)

$(TARGET): $(OBJS) 
	$(CC) $(CFLAGS) $(OPTFLAGS) -o $@ $(OBJS) $(LDFLAGS)

$(OBJS): $(HEADERS)

clean:
	rm -f *.o $(TARGET) *.log *~ *.lst
