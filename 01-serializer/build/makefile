OPTS = -Wall

bakery.o: ../include/bakery.hpp ../src/bakery.cpp
	g++ ${OPTS} -c ../include/bakery.hpp ../src/bakery.cpp

test: ../src/main.cpp bakery.o
	g++ ${OPTS} -o test bakery.o ../src/main.cpp

clean:
	rm -rf *.o test ../data/text_serialization.txt ../data/binary_serialization.bin
