SOURCES = model mainscreenview status statusview elementview element playerelement client1 client2
OBJECTS = $(SOURCES:%=%.o)

OBJECTS_AGE2 = $(filter-out client1.o, $(OBJECTS))
OBJECTS_AGE = $(filter-out client2.o, $(OBJECTS)) 

age: $(OBJECTS_AGE)
	g++ $(OBJECTS_AGE) -o age -lncurses

age2: $(OBJECTS_AGE2)
	g++ $(OBJECTS_AGE2) -o age2 -lncurses

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $<

.PHONY: clean

clean:
	rm *.o age age2
