client: ../src/client.cpp ../include/sender.hpp ../src/sender.cpp
	g++ -o client ../src/client.cpp ../src/sender.cpp ../include/sender.hpp -I ../third_party/asio-1.18.0/include/ -I ../include/ -pthread

server: ../src/server.cpp ../include/sender.hpp ../src/sender.cpp
	g++ -o server ../src/server.cpp ../src/sender.cpp ../include/sender.hpp -I ../third_party/asio-1.18.0/include/ -I ../include/ -pthread
