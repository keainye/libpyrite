#ifndef _PRT_SERVER_H
#define _PRT_SERVER_H

#include <mocutils/channel.h>

#include "define.h"
#include "functional"
#include "map"
#include "package.h"

namespace prt {
struct _client_data {
	int sequence;
	std::map<int, moc::channel<prt::package> *> promise_buf;
};

class server {
	int server_fd, sequence;
	sockaddr_in server_addr;
	std::map<
		std::string,
		std::function<
			bytes(
				sockaddr_in,
				bytes&,
				std::map<std::string, std::string>&
			)
		>
	> router;

 public:
	std::map<prt::bytes, _client_data> client_data;
	connection_state state;
	server(int port);
	~server();
	void start();
	void async();

	bool set_handler(
		std::string& identifier,
		std::function<
			bytes(
				sockaddr_in,
				bytes&,
				std::map<std::string, std::string>&
			)> handler
	);

	static void *process(void *_args);
	void tell(sockaddr_in client_addr, std::string identifier, bytes body, std::map<std::string, std::string> headers);
	bytes promise(sockaddr_in _client_addr, std::string identifer, bytes body, std::map<std::string, std::string> headers);
};
};	// namespace prt

#endif