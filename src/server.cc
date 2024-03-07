#include "server.h"

#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h>

#include "log.h"

prt::server::server(int port) {
  this->state = closed;
  if ((this->server_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    prt::panic("Pyrite bind port failed.");
 
  int len;  
  memset(&this->server_addr, 0, sizeof(struct sockaddr_in));
  this->server_addr.sin_family = AF_INET;
  this->server_addr.sin_port = htons(port);
  this->server_addr.sin_addr.s_addr = htonl(INADDR_ANY); 
  len = sizeof(this->server_addr);

  if (bind(this->server_fd, (struct sockaddr *) &this->server_addr, sizeof(this->server_addr)) < 0)
    prt::panic("Pyrite server binding failed.");

  this->sequence = 0;
  this->state = prt::established;
}


  // void start();
  // bool add_router(std::string identifier, std::function<bytes(int, bytes)> handler);
  // void tell(int client_id, std::string identifier, bytes body);
