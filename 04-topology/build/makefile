OPTS = -Wall

main: ../src/main.cpp ../src/manager.cpp ../src/router.cpp ../include/manager.hpp ../include/router.hpp
	g++ ${OPTS} -o main ../src/main.cpp ../src/manager.cpp ../src/router.cpp

manager.o: ../src/manager.cpp ../src/router.cpp ../include/manager.hpp ../include/router.hpp
	g++ ${OPTS} -o manager.o ../src/manager.cpp ../src/router.cpp
 
router.o: ../src/router.cpp ../src/router.cpp ../include/router.hpp ../include/manager.hpp
	g++ ${OPTS} -o router.o ../src/router.cpp ../src/manager.cpp
