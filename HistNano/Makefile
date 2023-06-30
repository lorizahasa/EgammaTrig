all: src/EventTree.o src/Selector.o makeHist 

makeHist:  src/Selector.o src/EventTree.o  makeHist.C
	g++ -o makeHist `root-config --libs` -I `root-config --incdir` -std=c++17 src/EventTree.o  src/Selector.o src/LumiMask.h makeHist.C 

src/EventTree.o: src/EventTree.cpp src/EventTree.h
	g++ -c -I `root-config --incdir` -std=c++17 src/EventTree.cpp -o src/EventTree.o

src/Selector.o: src/Selector.cpp src/Selector.h
	g++ -c -I `root-config --incdir` -std=c++17 src/Selector.cpp -o src/Selector.o 

clean:
	rm src/*.o 
