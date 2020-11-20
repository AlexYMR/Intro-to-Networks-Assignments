#include <inttypes.h>
#include <asio.hpp>
#include <ctime>
#include <iostream>
#include <string>
#include <map>

#include "../include/bakery.hpp"

using asio::ip::tcp;

int extractBits(int binary,int n,int pos);
uint16_t getEmployeeOrders(Bakery* bakery,uint8_t empNum); 
uint16_t getTotalOrders(Bakery* bakery); 
void addOrder(Bakery* bakery,uint8_t empNum,std::array<uint8_t,3> buffer); 

//TO DO:
/*
- [] figure out what uint16_t response = *reinterpret_cast<uint16_t*>(&buf.data()[i]); is doing
- [] figure out what parameter types you should be taking
- [] check if you need to error check at all?
- [] when do you use *something vs something->
*/

int main() {
  asio::io_context io_context;
  tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 3000));
  // Use this bakery to handle queries from the client
  Bakery bakery = binary_deserializer("../data/binary_serialization.bin");

  uint16_t counter = 0;
  
  while (true) {
    // Wait for client
    std::cout << "Yielding for client..." << std::endl;
    tcp::socket socket(io_context);
    acceptor.accept(socket);

    //response (total num of orders)
    //16-bit quantity (if M=00 or M=01)
    //for M=10, this response should be all 0s

    std::array<uint8_t, 3> buf;
    asio::error_code error;
    socket.read_some(asio::buffer(buf), error);

    int M=0,E=0;

    //M E Q1-Q5
    //M: 2, E: 2, Q1-Q5: 4 each

    M = extractBits(buf[0],2,6);
    E = extractBits(buf[0],2,4);
  
    std::array<uint8_t,2> response;
    response.fill(0);
    if(M == 1){ //M = 01
      //make sure works how you think: (uint16_t spills over 0 index into 1 index)
      response[0] = getEmployeeOrders(&bakery,E);
    } else if (M == 0){//M = 00
      response[0] |= getTotalOrders(&bakery);
    } else if (M == 2){//M = 10
      addOrder(&bakery,E,buf);
    } else{//cancel? abort?
      //won't client hang if I just continue?
      continue;
    }

    // Example of error handling
    // if (error != asio::error::eof)
    //   throw asio::system_error(error);

    // std::memcpy(&buf, &counter, sizeof(uint16_t));

    asio::write(socket, asio::buffer(response), error);
  }
  return 0;
}

//return k bits (as num) extracted from binary starting from position pos=1 from the left 
int extractBits(int binary, int k, int p) 
{ 
  return (binary >> p) & ((1 << k) - 1); 
} 

uint16_t getEmployeeOrders(Bakery* bakery, uint8_t empNum){
  if (empNum == 3) return 0; //throw error?

  int count = 0;
  //make mapping from binary to employee names
  std::map<uint8_t,std::string> employeeMap;
  employeeMap.insert(std::pair<uint8_t,std::string> (0,"Brad"));
  employeeMap.insert(std::pair<uint8_t,std::string> (1,"Claudia"));
  employeeMap.insert(std::pair<uint8_t,std::string> (1 << 1,"Simone"));
  for (unsigned long int i = 0; i < bakery->orders.size(); i++){
    if (bakery->orders[i].employee == employeeMap[empNum]){
      count++;
    }
  }

  return (uint16_t) count;
}

uint16_t getTotalOrders(Bakery* bakery){
  return (uint16_t) bakery->orders.size();
}

void addOrder(Bakery* bakery,uint8_t empNum,std::array<uint8_t,3> buffer){
  std::map<uint8_t,std::string> employeeMap;
  employeeMap.insert(std::pair<uint8_t,std::string> (0,"Brad"));
  employeeMap.insert(std::pair<uint8_t,std::string> (1,"Claudia"));
  employeeMap.insert(std::pair<uint8_t,std::string> (1 << 1,"Simone"));
  
  std::map<uint8_t,std::string> itemMap;
  itemMap.insert(std::pair<uint8_t,std::string> (0,"Biscuit"));
  itemMap.insert(std::pair<uint8_t,std::string> (1,"Bun"));
  itemMap.insert(std::pair<uint8_t,std::string> (2,"Brownie"));
  itemMap.insert(std::pair<uint8_t,std::string> (4,"White Loaf"));
  itemMap.insert(std::pair<uint8_t,std::string> (3,"Wheat Loaf"));

  //number of items wanted
  int arr[5] = {
  extractBits(buffer[0],4,0), //Biscuit (Q1)
  extractBits(buffer[1],4,4), //Bun (Q2)
  extractBits(buffer[1],4,0), //Brownie (Q3)
  extractBits(buffer[2],4,4), //White Loaf (Q4)
  extractBits(buffer[2],4,0), //Wheat Loaf (Q5)
  };

  Order order;
  order.employee = employeeMap[empNum];
  std::cout << "Adding order for " << order.employee << "..." << std::endl;
  for (int i = 0; i < 5; ++i){
    if(arr[i] != 0){
      std::pair<std::string,std::string> p = std::pair<std::string,std::string>(itemMap[i],std::to_string(arr[i]));
      order.items.push_back(p);
      std::cout << "Item: " << p.first << ", " << "Quantity: " << p.second << std::endl;
    }
  }

  bakery->orders.push_back(order);
}