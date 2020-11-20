client: ../src/client.cpp ../include/bakery.hpp ../src/bakery.cpp
	g++ -o client ../src/client.cpp ../src/bakery.cpp ../include/bakery.hpp -I ../third_party/asio-1.18.0/include/ -I ../include/ -pthread

server: ../src/server.cpp ../include/bakery.hpp ../src/bakery.cpp
	g++ -o server ../src/server.cpp ../src/bakery.cpp ../include/bakery.hpp -I ../third_party/asio-1.18.0/include/ -I ../include/ -pthread
