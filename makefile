CXXFLAGS_OPT=-O2
CXXFLAGS_WARN=-Wall -Wextra -Wno-unused-parameter -Wformat=2 -Wstrict-aliasing=2 -Wcast-qual -Wcast-align -Wwrite-strings -Wconversion -Wfloat-equal -Wpointer-arith -Wswitch-enum
CXXFLAGS=${CFLAGS_OPT} ${CFLAGS_WARN}

all:
	g++ ${CFLAGS} -o cpustat cpustat.cpp


clean:
	rm -f *.o
	rm -f cpustat
