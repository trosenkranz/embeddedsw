COMPILER=gcc
ARCHIVER=
CP=cp
COMPILER_FLAGS=-O3
EXTRA_COMPILER_FLAGS=
LIB=libxil.a

CC_FLAGS = $(COMPILER_FLAGS)
ECC_FLAGS = $(EXTRA_COMPILER_FLAGS)

RELEASEDIR=../../../lib
INCLUDEDIR=../../../include
INCLUDES=-I./. -I${INCLUDEDIR} -I../../../../lib/bsp/standalone/src/common -I../../common/src -I../../../../lib/bsp/standalone/src/common -I../../../../lib/bsp/standalone/src/arm/cortexr5 -I../../../../lib/bsp/standalone/src/arm/common/gcc/ -I../../../../lib/bsp/standalone/src/common -I../../../../XilinxProcessorIPLib/drivers/common/src
OUTS = *.o

LIBSOURCES:=*.c
INCLUDEFILES:=*.h

OBJECTS =	$(addsuffix .o, $(basename $(wildcard *.c)))

libs: banner xaxidma_libs clean

%.o: %.c
	${COMPILER} $(CC_FLAGS) $(ECC_FLAGS) $(INCLUDES) -o $@ $<

banner:
	echo "Compiling axidma"

xaxidma_libs: ${OBJECTS}
	$(ARCHIVER) -r ${RELEASEDIR}/${LIB} ${OBJECTS}

.PHONY: include
include: xaxidma_includes

xaxidma_includes:
	${CP} ${INCLUDEFILES} ${INCLUDEDIR}

clean:
	rm -rf ${OBJECTS}
