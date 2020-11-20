#include <asio.hpp>
#include <iostream>

using asio::ip::tcp;

int main() {
  asio::io_context io_context;
  tcp::resolver resolver(io_context);
  tcp::resolver::results_type endpoints = resolver.resolve("127.0.0.1", "3000");

  tcp::socket socket(io_context);
  asio::connect(socket, endpoints);

  asio::error_code error;

  //M E Q1-Q5
  //M: 2, E: 2, Q1-Q5: 4 each

  //request #1
  //Get the number of orders for E=01 (Claudia)
  std::array<uint8_t, 3> buf;
  buf.fill(0);
  buf[0] |= 101 << 4; //01010000 0s for rest
  asio::write(socket, asio::buffer(buf), error);
  //response
  socket.read_some(asio::buffer(buf), error);
  uint16_t response = *reinterpret_cast<uint16_t*>(&buf.data()[0]);
  std::cout << "Claudia has: " << response << " orders" << std::endl;

  
  //request #2
  //Add an order: E=01, Q1=2, Q2=5, Q3=0, Q4=1, Q5=0
  std::cout << "Adding order for Claudia...";
  buf.fill(0);
  asio::connect(socket, endpoints);
  buf[0] |= ((1 << 7) | (1 << 4) | 1 << 1); //10 01 0001 
  buf[1] |= ((1 << 6) | (1 << 4)); //0101 0000 
  buf[2] |= 1 << 4; //0001 0000
  asio::write(socket, asio::buffer(buf), error);
  //response
  socket.read_some(asio::buffer(buf), error);
  std::cout << " order added!" << std::endl;

  //request #3
  //Get the number of orders for E=01 (should have increased by one!)
  buf.fill(0);
  asio::connect(socket, endpoints);
  buf[0] |= 101 << 4; //01010000 0s for rest
  asio::write(socket, asio::buffer(buf), error);
  //response
  socket.read_some(asio::buffer(buf), error);
  response = *reinterpret_cast<uint16_t*>(&buf.data()[0]);
  std::cout << "Claudia now has: " << response << " orders" << std::endl;

  //request #4
  //Get the total number of orders (all 3 bytes 0)
  buf.fill(0);
  asio::connect(socket, endpoints);
  asio::write(socket, asio::buffer(buf), error);
  //response
  socket.read_some(asio::buffer(buf), error);
  response = *reinterpret_cast<uint16_t*>(&buf.data()[0]);
  std::cout << "The total number of orders is: " << response << std::endl;

  return 0;
}