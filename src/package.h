#ifndef _PRT_PACKAGE
#define _PRT_PACKAGE

#include <sys/types.h>

#include "string"
#ifdef Windows
#include <winsock2.h>
#include <windows.h>
#include <winsock2.h>
typedef int socklen_t;
#define MSG_CONFIRM 0
#pragma comment(lib, "ws2_32.lib")
#else
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#endif

#include "define.h"
#include "mocutils/byte.h"

namespace prt {
typedef moc::bytes bytes;
class package {
 public:
	i32         sequence;   // valid for promise (-1, when tell)
	std::string identifier; // for handler router

	std::map<std::string, std::string> headers;

	bytes body;

	package();
	package(bytes raw);
	package(package *old);
	package(i32 sequence, std::string identifier, bytes body);
	bytes to_bytes();
	bool operator==(package &other);
	bool operator!=(package &other);
	bool operator<(prt::package &other);

	void set_body(std::string text);
	std::string body_as_string();
	void send_to(int socket_fd, sockaddr_in socket_addr);

	void debug_print();
};

typedef struct {
	void *ptr;
	sockaddr_in addr;
	package pkg;
} process_args;
}	 // namespace prt

#endif