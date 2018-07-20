CC        = cc
CFLAGS    = -O3 -lm
OPTFLAGS  = -O3 -lm
LDFLAGS   = 

TARGET    = ../run/L2-color

.SUFFIXES:
.SUFFIXES: .o .c 

.c.o:
	$(CC) -c $(CFLAGS) $(OPTFLAGS) $< -o $@

OBJS = \
	pointer_init.o \
	mc.o \
	cm.o \
	rcm.o \
	cmrcm.o \
	poi_gen.o \
	outucd.o \
	main.o

HEADERS = \
	struct.h \
	struct_ext.h \
	pcg.h \
	pcg_ext.h \
	pointer_init.h \
	mc.h \
	cm.h \
	rcm.h \
	cmrcm.h \
	poi_gen.h \
	outucd.h

all: $(TARGET)

$(TARGET): $(OBJS) 
	$(CC) $(CFLAGS) $(OPTFLAGS) -o $@ $(OBJS) $(LDFLAGS)

$(OBJS): $(HEADERS)

clean:
	rm -f *.o $(TARGET) *~ *.log
