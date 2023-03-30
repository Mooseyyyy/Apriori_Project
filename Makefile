
CCC= g++
CCCFLAGS = -Wall -std=c++11

all : GenDatabase apriori idea1
database : GenDatabase.o
apriori : Apriori.o
idea1 : Idea1.cc
	$(CCC) $(CCCFLAGS) $^ -o $@

# default rule for compiling .cc to .o
%.o: %.cc
	$(CCC) -c $(CCCFLAGS) $<

clean:
	rm -f *.o *~ *% *# .#*

clean-all: clean
	rm -f database apriori
