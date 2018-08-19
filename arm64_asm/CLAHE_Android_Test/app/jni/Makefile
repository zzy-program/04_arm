CC= g++
CXXFLAGS = -I./tools -g -pg -Ofast#-Ofast -msse3 -ftree-vectorize -fopt-info-vec-optimized
CCFLAGS = $(CXXFLAGS)
CLDFLAGS = -lm
VPATH = ./:./tools

all: test_clahe test_clahe
test_clahe: test_clahe.o realtime_clahe.o Histogram.o log.o tictoc.o QImage.o
	$(CC) -g -pg $^ -o $@
test_hist: test_hist.o  Histogram.o log.o tictoc.o

clean:
	rm *.o
.PHONY: clean
