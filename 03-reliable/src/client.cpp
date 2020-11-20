#include <asio.hpp>
#include <iostream>
#include "../include/sender.hpp"

#include <map>
#include <chrono>
#include <fstream>

using asio::ip::tcp;

namespace sc = std::chrono;

// This one was hard, Brad! Although, most of it's because it's C++, to be honest...
// Watching the cursor blink was super boring and uninsighful, so I left my debugging outputs in--I think they're nice.
// I also made the server segfault a few times, but we don't talk about that

//check if vector contains an element: return true/false and position if so
template <typename T,typename T2>
std::pair<bool,int> vectorContains(std::vector<T> vec,T2 id){
  typename std::vector<T>::iterator it; //why do I need typename here? It was talking about a "dependent scope?"
  for(it = vec.begin(); it != vec.end(); ++it){
    if(it->first == id){
      //it - vec.begin() gives the current it index
      return std::pair<bool,int> (true,it - vec.begin());
    }
  }
  return std::pair<bool,int> (false,0);
}

int main() {
  asio::io_context io_context;
  Sender sender(io_context, "127.0.0.1", "3000");

  // An explanation of the API
  // data_ready(): returns true if a message is available, otherwise false
  // get_msg(): gets an available message
  // request_msg(id): requests a message at id [0, NUM_MSGS)

  //maps keep things ordered, so we can just use them to solve our ordering problem (assuming no loss)
  std::map<int,std::string> messages;

  //maintains (msg_id,second) pairs for timeout control
  std::vector<std::pair<uint16_t,sc::_V2::system_clock::time_point>> timeouts;
  
  const size_t LAST_CHUNK = 852; //defines last chunk to be requested
  const size_t WINDOW_SIZE = 10; //defines max size of the request queue

  int TIMEOUT = 1;
  int last_frame = 0;

  do{
    //request at most 10 messages without waiting for feedback
    if(timeouts.size() < WINDOW_SIZE){
      if(last_frame <= LAST_CHUNK){
        sender.request_msg(last_frame);
        sc::_V2::system_clock::time_point start = sc::high_resolution_clock::now();
        timeouts.push_back(std::pair<uint16_t,sc::_V2::system_clock::time_point> (last_frame,start));
        last_frame++;
      }
    }

    //check if there're any messages to retrieve
    if(sender.data_ready()){
      auto msg = sender.get_msg();
      std::vector<std::pair<uint16_t,sc::_V2::system_clock::time_point>>::iterator it = timeouts.begin();

      //check if this message hasn't been received (vector contains the item)
      std::pair<bool,int> result = vectorContains(timeouts,msg.msg_id);
      if(result.first){
        advance(it,result.second);
        timeouts.erase(it);

        std::cout << "Current timers: " << timeouts.size() << std::endl;

        //if message received, make sure it isn't a duplicate
        if(!messages.count(msg.msg_id)){ //1 if found, 0 if not (check if key not in map, avoid duplicates)
          std::string data_str = std::string(msg.data.data(), CHUNK_SIZE);
          messages.insert(std::pair<uint16_t,std::string> (msg.msg_id,data_str));
          std::cout << "Successfully added a message to buffer!" << std::endl;
        }
      }
    }

    //go through remainder of timeouts' elements (those chunks that weren't received)
    //and check if their timeouts have been realized--resend request for it if so
    std::vector<std::pair<uint16_t,sc::_V2::system_clock::time_point>>::iterator it;
    for(it = timeouts.begin(); it != timeouts.end(); ++it){
      if((sc::duration_cast<sc::seconds> (sc::high_resolution_clock::now() - it->second).count()) >= TIMEOUT){
        std::cout << "Re-requesting chunk #" << it->first << std::endl;
        sender.request_msg(it->first);
        it->second = sc::high_resolution_clock::now();
      }
    }

  } while (timeouts.size() > 0);

  std::cout << "Writing results to file..." << std::endl;

  //write to a text file
  std::ofstream write;
  write.open("../data/gilgameshExtraction.txt", std::ios_base::out | std::ios_base::trunc);
  if(write.is_open()){

    //write results
    std::map<int,std::string>::iterator it;
    for(it = messages.begin(); it != messages.end(); ++it){
      write << it->second;
    }
    write.close();

  }
  return 0;
}


/* 
5-packet stop-and-wait -- relic

while (curr_msg <= LAST_CHUNK) {
    do{
      //request at most 5 chunks without waiting for feedback
      outstandingRequests = 0;
      int chunks_size = (LAST_CHUNK - (last_frame - 1) >= WINDOW_SIZE) ? WINDOW_SIZE : LAST_CHUNK - (last_frame - 1);
      for(int i = first_frame; i < first_frame + chunks_size; i++){
        sender.request_msg(i);
        outstandingRequests++;
        last_frame++;
      }

      //start timer (1 second), wait for responses
      auto start = std::chrono::high_resolution_clock::now();
      while ((std::chrono::duration_cast<std::chrono::seconds> (std::chrono::high_resolution_clock::now() - start).count()) < 1){
        if(sender.data_ready()){
          auto msg = sender.get_msg();
          //check if this message hasn't been received
          if(msg.msg_id >= first_frame && msg.msg_id < last_frame){
            outstandingRequests--;
            // std::cout << "now outstanding: " << outstandingRequests << std::endl;

            //if message received, make sure it isn't a duplicate
            if(!messages.count(msg.msg_id)){ //1 if found, 0 if not (check if key in map, avoid duplicates)
              std::string data_str = std::string(msg.data.data(), CHUNK_SIZE);
              messages.insert(std::pair<uint16_t,std::string> (msg.msg_id,data_str));
              curr_msg++;
              // std::cout << "successfully added a message to thingy" << std::endl;
            }

            //don't wait whole second if no drops
            if(outstandingRequests == 0){
              first_frame = last_frame;
              break;
            }

          }
        }
      }
      
      // std::cout << "this is just a print" << std::endl;

      if(outstandingRequests > 0){
        last_frame -= chunks_size;
      }

    } while (outstandingRequests > 0);

    // std::cout << "this is just another a print" << std::endl;
  }

 */