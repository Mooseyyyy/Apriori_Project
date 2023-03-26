
CCC= g++
CCCFLAGS = -Wall -std=c++11

all : database apriori
database : GenDatabase.o
apriori : Apriori.o
	$(CCC) $(CCCFLAGS) $^ -o $@

# default rule for compiling .cc to .o
%.o: %.cc
	$(CCC) -c $(CCCFLAGS) $<

clean:
	rm -f *.o *~ *% *# .#*

clean-all: clean
	rm -f database apriori
