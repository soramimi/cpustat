
CXXFLAGS = -std=c++11 -O2

all: cpustat

cpustat: cpustat.o
	g++ -o $@ $^

clean:
	rm -f *.o
	rm -f cpustat
