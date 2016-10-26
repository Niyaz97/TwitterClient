#ifndef TWITTERCLIENT_CLIENT_HPP
#define TWITTERCLIENT_CLIENT_HPP

#include <string>
#include <map>

namespace Twitter {
	class Client {
	public:
		using dict_t = std::map<std::string, std::string>;
   		Client(dict_t settings);
   		auto check_connection() -> bool;
	};
}


#endif //TWITTERCLIENT_CLIENT_HPP
