COMPILER=gcc
ARCHIVER=
CP=cp
COMPILER_FLAGS=-O3
EXTRA_COMPILER_FLAGS=
LIB=libxil.a

RELEASEDIR=../../../lib
INCLUDEDIR=../../../include
INCLUDES=-I./. -I${INCLUDEDIR} -I../../../../lib/bsp/standalone/src/common -I../../common/src -I../../../../lib/bsp/standalone/src/common -I../../../../lib/bsp/standalone/src/arm/cortexr5 -I../../../../lib/bsp/standalone/src/arm/common/gcc/ -I../../../../lib/bsp/standalone/src/common -I../../../../XilinxProcessorIPLib/drivers/common/src
OUTS = *.o
OBJECTS =	$(addsuffix .o, $(basename $(wildcard *.c)))

LIBSOURCES=*.c
INCLUDEFILES=*.h

libs:
	echo "Compiling axidma"
	$(COMPILER) $(COMPILER_FLAGS) $(EXTRA_COMPILER_FLAGS) $(INCLUDES) $(LIBSOURCES)
	$(ARCHIVER) -r ${RELEASEDIR}/${LIB} $(OBJECTS)
	make clean

include:
	${CP} $(INCLUDEFILES) $(INCLUDEDIR)

clean:
	rm -rf $(OBJECTS)
