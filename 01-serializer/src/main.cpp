#include <chrono>
#include <iostream>
#include <fstream>
#include "../include/bakery.hpp"
using namespace std::chrono;

int main() {
  std::cout << "\nStarting...\n" << std::endl;
  auto bakery = text_deserializer("../data/bakery.txt");
  // auto start = high_resolution_clock::now();
  // auto end_time = high_resolution_clock::now() - start;
  // print_bakery(bakery);
  // std::cout << "\nText deserializer took: "
  //           << duration_cast<milliseconds>(end_time).count() << "ms\n"; 
  //

  std::cout << "\n=============\n\nTesting text serializer...\n\n";
  
  auto start = high_resolution_clock::now();
  text_serializer(bakery,"../data/text_serialization.txt");
  auto end_time = high_resolution_clock::now() - start;

  std::cout << "Took: " << duration_cast<milliseconds> (end_time).count() << "ms\n";

  std::cout << "\n=============\n\nTesting binary serializer...\n\n";

  start = high_resolution_clock::now();
  binary_serializer(bakery,"../data/binary_serialization.bin");
  end_time = high_resolution_clock::now() - start;

  std::cout << "Took: " << duration_cast<milliseconds> (end_time).count() << "ms\n";
  
  std::cout << "\n=============\n\nTesting binary deserializer...\n\n";

  start = high_resolution_clock::now();
  Bakery B = binary_deserializer("../data/binary_serialization.bin");
  end_time = high_resolution_clock::now() - start;
  //print_bakery(B);
  std::cout << "\nTook: " << duration_cast<milliseconds> (end_time).count() << "ms\n";

  return 0;
}