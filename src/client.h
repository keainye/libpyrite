#ifndef _PRT_CLIENT_H
#define _PRT_CLIENT_H

#include <mocutils/channel.h>

#include "define.h"
#include "functional"
#include "map"
#include "package.h"

#define PRT_CLIENT_HANDLER(func_name) prt::bytes func_name( \
	prt::bytes, \
	std::map<std::string, std::string>&)

namespace prt {
class client {
	int server_fd;
	sockaddr_in server_addr;
	std::map<std::string, std::function<bytes(bytes, std::map<std::string, std::string>&)>> router;
	int sequence;
	std::map<int, moc::channel<prt::package> *> promise_buf;

 public:
	connection_state state;
	client(const char *ip, int port);
	~client();
	void start();
	void async();
	bool set_handler(std::string identifier, std::function<bytes(bytes, std::map<std::string, std::string>&)> handler);
	static void *process(void *_args);
	void tell(std::string identifier, bytes body, std::map<std::string, std::string> headers);
	bytes promise(std::string identifer, bytes body, std::map<std::string, std::string> headers);
};
}	 // namespace prt

#endif