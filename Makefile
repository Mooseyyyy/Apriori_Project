
CCC= g++
CCCFLAGS = -Wall -std=c++11

all : GenDatabase apriori idea1
database : GenDatabase.o
	$(CCC) $(CCCFLAGS) $^ -o $@
apriori : Apriori.o
	$(CCC) $(CCCFLAGS) $^ -o $@
idea1 : Idea1.o
	$(CCC) $(CCCFLAGS) $^ -o $@

# default rule for compiling .cc to .o
%.o: %.cc
	$(CCC) -c $(CCCFLAGS) $<

clean:
	rm -f *.o *~ *% *# .#*
clean-apriori:
	rm -f apriori Apriori.o
clean-idea1:
	rm -f idea1 Idea1.o
clean-all: clean
	rm -f database apriori idea1 *K.txt
