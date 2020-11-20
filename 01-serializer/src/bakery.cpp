#include "../include/bakery.hpp"
#include <fstream>
#include <iostream>
#include <cmath>
#include <sstream>
#include <map>

using namespace std;

void print_bakery(const Bakery& bakery) {
  std::cout << "Employees: " << std::endl;
  for (auto employee : bakery.employees) {
    std::cout << employee << std::endl;
  }

  std::cout << std::endl;
  std::cout << "Items: " << std::endl;
  for (auto item : bakery.items) {
    std::cout << item.name << ", " << item.price << std::endl;
  }

  std::cout << std::endl;
  std::cout << "Orders: " << std::endl;
  for (auto order : bakery.orders) {
    std::cout << order.employee << ": ";
    auto j = 0;
    for (auto item : order.items) {
      std::cout << item.second << " " << item.first;
      j++;
      if (size_t(j) < order.items.size())
        std::cout << ", ";
    }
    std::cout << std::endl;
  }
}

// You shouldn't need to edit this function (unless you want!)
Bakery text_deserializer(std::string file_path) {
  std::ifstream infile(file_path);
  std::string line;
  Bakery bakery;

  while (!infile.eof()) {
    // Employees section
    if (line.compare("@employees") == 0) {
      std::getline(infile, line);
      while (line.size() > 0) {
        bakery.employees.push_back(line);
        std::getline(infile, line);
      }
    }

    // Items section
    if (line.compare("@items") == 0) {
      std::getline(infile, line);
      while (line.size() > 0) {
        auto end = line.find(", ");
        Item item;
        item.name = line.substr(0, end);
        item.price = line.substr(end + 2);
        bakery.items.push_back(item);
        std::getline(infile, line);
      }
    }

    // Orders section
    if (line.compare("@orders") == 0) {
      std::getline(infile, line);
      auto e = bakery.employees;
      while (line.size() > 0) {
        Order order;
        auto end = line.find(": ");
        order.employee = line.substr(0, end);

        // Find all the orders
        auto rest = line.substr(end + 2);
        size_t pos = 0;
        std::string token;
        while ((pos = rest.find(", ")) != std::string::npos) {
          token = rest.substr(0, pos);
          end = token.find(" ");
          auto quantity = token.substr(0, end);
          auto item_name = token.substr(end + 1);
          order.items.push_back(std::make_pair(item_name, quantity));
          rest.erase(0, pos + 2);
        }
        end = rest.find(" ");
        auto quantity = rest.substr(0, end);
        auto item_name = rest.substr(end + 1);
        order.items.push_back(std::make_pair(item_name, quantity));
        bakery.orders.push_back(order);

        // no more lines to read
        if (infile.eof())
          break;

        std::getline(infile, line);
      }
    }

    std::getline(infile, line);
  }

  return bakery;
}

void text_serializer(const Bakery& bakery, std::string file_path){
  //given bakery struct, output to file_path in the format of files in ../data
  ostringstream employees,items,orders;
  employees << "@employees\n";
  items << "@items\n";
  orders << "@orders\n";

  //employees
  for(unsigned long int i = 0; i < bakery.employees.size(); ++i){
    employees << bakery.employees[i] << "\n";
  }

  //items
  for(unsigned long int i = 0; i < bakery.items.size(); ++i){
    items << bakery.items[i].name << ", " << bakery.items[i].price << "\n";
  }

  for(unsigned long int i = 0; i < bakery.orders.size(); ++i){
    orders << bakery.orders[i].employee << ": ";
    int k = 0;
    for(unsigned long int j = 0; j < bakery.orders[i].items.size(); ++j){
      orders << bakery.orders[i].items[j].second << " " << bakery.orders[i].items[j].first;
      k++;
      //what does size_t do?
      if(size_t(k) < bakery.orders[i].items.size()){
        orders << ", ";
      } 
    }
    orders << endl;
  }

  orders << "\n";
  
  //output the streams onto a file in file_path
  ofstream write;
  write.open(file_path, ios_base::out | ios_base::trunc);
  if(write.is_open()){
    write << employees.str() << endl
       << items.str() << endl
       << orders.str();
    write.close();
  }
}

/* string decimal_to_binary(int n){
  string remainder;
  while(n != 0){
    remainder = (n % 2 == 0 ? "0" : "1") + remainder;
    n = n / 2;
  }
  return string(remainder.rbegin(),remainder.rend());
}

int binary_to_decimal(string b){
  int n = 0;
  for(unsigned long int i = 0; i < b.length(); ++i){
    n += pow(2,b.length() - i)*(b[i] - '0');
  }
  return n;
}

string string_to_binary(string s){
  string out;
  for(unsigned long int i = 0; i < s.length(); ++i){
    out += decimal_to_binary(int(s[i]));
  }
  return out;
} */

void binary_serializer(const Bakery& bakery, std::string file_path){
  ofstream f;
  f.open(file_path, ios_base::binary | ios_base::trunc);
  if(f.is_open()){

    short int keyMapIndex = 1;
    map<string,short int> keyMap; //will make storage more efficient for orders

    short int keyMapIndex2 = 1;
    map<string,short int> keyMap2;

    //employees
    short int emp_size = bakery.employees.size();
    f.write(reinterpret_cast<char*>(&emp_size),sizeof(short int));
    for(unsigned long int i = 0; i < bakery.employees.size(); ++i){
      int string_size = bakery.employees[i].size();
      f.write(reinterpret_cast<const char*>(&string_size),sizeof(int));
      f.write(bakery.employees[i].c_str(),string_size);

      keyMap.insert(pair<string,short int> (bakery.employees[i],keyMapIndex));
      keyMapIndex++;
    }
    
    //items
    short int itms_size = bakery.items.size();
    f.write(reinterpret_cast<char*>(&itms_size),sizeof(short int));
    for(unsigned long int i = 0; i < bakery.items.size(); ++i){
      Item itm = bakery.items[i];
      int name_size = itm.name.size();
      f.write(reinterpret_cast<const char*>(&name_size),sizeof(int));
      f.write(itm.name.c_str(),name_size);

      int price_size = itm.price.size();
      f.write(reinterpret_cast<const char*>(&price_size),sizeof(int));
      f.write(itm.price.c_str(),price_size);

      keyMap2.insert(pair<string,short int>(itm.name,keyMapIndex2));
      keyMapIndex2++;
    }

    //orders
    int orders_size = bakery.orders.size();
    f.write(reinterpret_cast<char*>(&orders_size),sizeof(int));
    for(unsigned long int i = 0; i < bakery.orders.size(); ++i){
     
      Order order = bakery.orders[i];
      //use keyMap (store mapped int)
      f.write(reinterpret_cast<char*>(&keyMap.find(order.employee)->second),sizeof(short int));

      int order_items_size = order.items.size();
      f.write(reinterpret_cast<char*>(&order_items_size),sizeof(int));
      for(unsigned long int j = 0; j < order.items.size(); ++j){
        pair<string,string> p = order.items[j];

        //quantity (store as char, since number is between 0 and 9)
        f.write(p.second.c_str(),sizeof(char));

        //item name -- use keyMap (store mapped int)
        f.write(reinterpret_cast<char*>(&keyMap2.find(p.first)->second),sizeof(short int));
      }
    }

    f.close();
  }
}

Bakery binary_deserializer(std::string file_path){
  ifstream f;
  vector<string> employees;
  vector<Item> items;
  vector<Order> orders;
  f.open(file_path, ios_base::binary | ios_base::in);
  if(f.is_open()){
    
    short int keyMapIndex = 1;
    map<short int,string> keyMap; //makes storage more efficient for orders
   
    short int keyMapIndex2 = 1;
    map<short int,string> keyMap2;

    //employees
    short int emp_size;
    f.read(reinterpret_cast<char*>(&emp_size),sizeof(short int));
    for(int i = 0; i < emp_size; i++){
      int string_size;
      string s;
      f.read(reinterpret_cast<char*>(&string_size),sizeof(int));
      s.resize(string_size);
      f.read(&s[0],string_size);
      employees.push_back(s);

      keyMap.insert(pair<short int,string>(keyMapIndex,s));
      keyMapIndex++;
    }

    //items
    short int itms_size;
    f.read(reinterpret_cast<char*>(&itms_size),sizeof(short int));
    for(int i = 0; i < itms_size; i++){
      int name_size,price_size;
      string n,p;
      
      f.read(reinterpret_cast<char*>(&name_size),sizeof(int));
      n.resize(name_size);
      f.read(&n[0],name_size);
      
      f.read(reinterpret_cast<char*>(&price_size),sizeof(int));
      p.resize(price_size);
      f.read(&p[0],price_size);

      Item itm;
      itm.name = n;
      itm.price = p;
      items.push_back(itm);

      keyMap2.insert(pair<short int,string>(keyMapIndex2,n));
      keyMapIndex2++;
    }

    //orders
    int orders_size;
    f.read(reinterpret_cast<char*>(&orders_size),sizeof(int));
    for(int i = 0; i < orders_size; i++){
      int emp_name_value,order_items_size;

      //get string val from kepMap
      f.read(reinterpret_cast<char*>(&emp_name_value),sizeof(short int));
      string emp_name = keyMap.find(emp_name_value)->second;

      vector<pair<string,string>> order_items;
      f.read(reinterpret_cast<char*>(&order_items_size),sizeof(int));
      for(int j = 0; j < order_items_size; ++j){
        pair<string,string> p;
        int item_name_value;
        char quantity;
        string item_quantity;

        //quantity
        f.read(&quantity,sizeof(char));
        item_quantity.resize(sizeof(char));
        item_quantity = to_string(quantity - '0');

        //item name (get string name from keyMap)
        f.read(reinterpret_cast<char*>(&item_name_value),sizeof(short int));
        string item_name = keyMap2.find(item_name_value)->second;

        p.first = item_name;
        p.second = item_quantity;
        order_items.push_back(p);
      }
      Order order;
      order.employee = emp_name;
      order.items = order_items;
      orders.push_back(order);
    }

    f.close();
  }

  Bakery B;
  B.employees = employees;
  B.items = items;
  B.orders = orders;

  return B;
}